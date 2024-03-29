#include <iostream>
#include <algorithm>
#include "PathTracer.h"
#include "MyMath.h"
#include "Ray.h"
#include "Color.h"
#include "Material/Material.h"
#include "stb_image_write.h"
#include <omp.h>
#include <cassert>



inline bool PathTracer::m_WritePixelRadiance(unsigned int x_id, unsigned int y_id, Vector3& radiance)
{
	SimplePT::GammaCorrection(radiance);
	Color3 pixel_color(radiance.m_x, radiance.m_y, radiance.m_z);
	return m_WritePixelColor(x_id, y_id, pixel_color);
}

inline bool PathTracer::m_WritePixelColor(unsigned int x_id, unsigned int y_id, const Color3& col)
{
	if (m_frame_buffer == nullptr)
	{
		std::clog << "frambuffer is nullptr" << std::endl;
		return false;
	}

	size_t pixel_index = static_cast<size_t>(m_camera.GetWidth()) * y_id + x_id;
	m_frame_buffer[pixel_index * 3 + 0] = col.R();
	m_frame_buffer[pixel_index * 3 + 1] = col.G();
	m_frame_buffer[pixel_index * 3 + 2] = col.B();

	return true;
}

void PathTracer::Render(int num_samples_per_pixel)
{
	std::clog << "start rendering" << std::endl;
	std::clog << "SPP = " << num_samples_per_pixel << std::endl;
	int debug_pixel_x = 50;
	int debug_pixel_y = 50;
	//int debug_pixel_y = 13;
	
	const int width = m_camera.GetWidth();
	const int height = m_camera.GetHeight();

	int linecnt = 0;
#ifdef OMP_PT
	#pragma omp parallel for 
#endif // OMP_PT
	for (int j = 0; j < height; ++j)
	{
		if (linecnt % 50 == 1)
			std::clog << "\rline: " << linecnt << '/' << height - 1 << "     " << std::endl;
		for (int i = 0; i < width; ++i)
		{
#ifdef DEBUG_PT
			/* debug: get processing image for debuggging */
			if (i == debug_pixel_x && j == debug_pixel_y)
			{
				m_WritePixelColor(i, j, Color3(255u, 0u, 0u)); // red pixel
				stbi_write_png("image/debug.png", width, height, CHANNEL_NUM, m_frame_buffer.get(), width * CHANNEL_NUM);
			}
#endif // DEBUG_PT

			Vector3 pixel_color_radiance(0.0, 0.0, 0.0);
			for (int sppcnt = 0; sppcnt < num_samples_per_pixel; ++sppcnt)
			{
				Ray eye_ray = m_camera.CastRay(i, j); // generate ray from eye
				pixel_color_radiance +=  m_RayRadiance(eye_ray) / num_samples_per_pixel;
			}
			m_WritePixelRadiance(i, j, pixel_color_radiance);
		}
#ifdef OMP_PT
		#pragma omp atomic
#endif // OMP_PT
		linecnt++;
	}

	// if CHANNEL_NUM is 4, you can use alpha channel in png
	stbi_write_png("image/stbpng.png", width, height, CHANNEL_NUM, m_frame_buffer.get(), width * CHANNEL_NUM);

}

// Evaluate Rendering Equation
// reference: https://agraphicsguynotes.com/posts/basics_about_path_tracing/
// L(x1 -> x0) = Le(x1 -> x0)
//					+ \int L(x2 -> x1) * f(x2 -> x1 -> x0) * cos1 * dw1 
//	           = Le(x1 -> x0) 
//					+ \int Le(x2 -> x1) * f(x2 -> x1 -> x0) * cos1 * dw1 
//					+ \int (\int L(x3 -> x2) * f(x3 -> x2 -> x1) * cos2 * dw2) * f(x2 -> x1 -> x0) * cos1 * dw1
Vector3 PathTracer::m_RayRadiance(const Ray& ray) const
{
	HitRecord hit_record;
	Vector3 out_radiance;

	// miss
	if (!m_scene.HitHappened(ray, hit_record))
		return Vector3(); // defualt radiance (black)

	/* first term: Le(x1 -> x0) */
	if (hit_record.m_material.HasEmission())
	{
		Vector3 emit_radiance(hit_record.m_material.GetEmission());
		out_radiance += emit_radiance;
		return out_radiance; // eye ray hit light
	}

	// surface info
	Vector3 kd = hit_record.m_material.GetKd(); 
	Vector3 BRDF = kd / SimplePT::PI; 

	/* second term: \int le(x2 -> x1) * f(x2 -> x1 -> x0) * cos(\theta) dw */
	// direct = brdf * (n * w) * (n' * w') * light_emmit / r ^ 2 / p; (integral on da)
	HitRecord light_surface_info;
	double pdf_all_light;
	m_scene.SampleLight(light_surface_info, pdf_all_light);

	Vector3 dir2light = light_surface_info.m_hit_position - hit_record.m_hit_position;
	Vector3 unit_dir2light = dir2light.Normalized();
	Vector3 direct_illum_radiance;
	if (m_Visible(hit_record.m_hit_position, light_surface_info.m_hit_position))
	{
		double r2 = dir2light.SquareLength();
		double cosi = std::max(0.0, DotProduct(hit_record.m_hit_unit_normal, unit_dir2light));
		double cosl = std::max(0.0, DotProduct(light_surface_info.m_hit_unit_normal, -unit_dir2light));
		direct_illum_radiance = light_surface_info.m_material.GetEmission() * BRDF  * cosi * cosl / r2 / pdf_all_light;
		out_radiance += direct_illum_radiance;
	}

	/* sampling no light */
	// indir = brdf * (n * w) * m_RayRadiance(random_wi_on_hemisphere) / p  (integral on dwi)
	Vector3 indirect_illum_radiance;
	double RR = 0.8;
	if (SimplePT::GetRandomDouble_0_to_1() < RR)
	{
		Vector3 wi;
		double pdf_of_wi = -1.0;
		SimplePT::Sample_Hemisphere_Cos_Weighted(hit_record.m_hit_unit_normal, wi, pdf_of_wi);
		assert(pdf_of_wi > 0.0 && pdf_of_wi < 1.0);
		Ray ray_wi(hit_record.m_hit_position, wi);

		HitRecord next_obj_rec;
		if (m_scene.HitHappened(ray_wi, next_obj_rec)
			&& !next_obj_rec.m_material.HasEmission())
		{
			double cosi = std::max(0.0, DotProduct(hit_record.m_hit_unit_normal, wi));
			indirect_illum_radiance = m_RayRadiance(ray_wi) * BRDF * cosi / pdf_of_wi / RR;
			out_radiance += indirect_illum_radiance;
		}

	}

	return out_radiance;
}

bool PathTracer::m_Visible(const Position3& pos0, const Position3& pos1) const 
{
	Vector3 dir = pos1 - pos0;
	Vector3 unit_dir = dir.Normalized();
	Ray shadow_ray(pos0 + SimplePT::EPSILON * unit_dir, unit_dir);
	HitRecord block_rec;
	m_scene.HitHappened(shadow_ray , block_rec);
	Vector3 offset = pos1 - block_rec.m_hit_position;

	return offset.Length() < 0.01; // close enough to light
}
