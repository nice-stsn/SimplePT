#include <iostream>
#include "PathTracer.h"
#include "MyMath.h"
#include "Ray.h"
#include "Color.h"
#include "Material/Material.h"
#include "stb_image_write.h"



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
	
	const int width = m_camera.GetWidth();
	const int height = m_camera.GetHeight();

	int index = 0;
	for (int j = 0; j < height; ++j)
	{
		std::clog << "\rline: " << j << '/' << height - 1 ;
		for (int i = 0; i < width; ++i)
		{
			float r = (float)i / (float)width;
			float g = (float)j / (float)height;
			float b = 0.2f;
			
			Color3 pixel_color(0u, 0u, 0u);
			Ray eye_ray = m_camera.CastRay(i, j); // generate ray from eye
			pixel_color += m_RayColor(eye_ray);

			m_WritePixelColor(i, j, pixel_color);
///* get processing image for debuggging */
//// if CHANNEL_NUM is 4, you can use alpha channel in png
//stbi_write_png("image/debug.png", width, height, CHANNEL_NUM, m_frame_buffer.get(), width * CHANNEL_NUM);
		}
	}

	// if CHANNEL_NUM is 4, you can use alpha channel in png
	stbi_write_png("image/stbpng.png", width, height, CHANNEL_NUM, m_frame_buffer.get(), width * CHANNEL_NUM);

	//// You have to use 3 comp for complete jpg file. If not, the image will be grayscale or nothing.
	//stbi_write_jpg("image/stbjpg3.jpg", width, height, 3, m_frame_buffer.get(), 100);
}

// recursive call
// reference: https://agraphicsguynotes.com/posts/basics_about_path_tracing/
// L(x1 -> x0) = Le(x1 -> x0)
//					+ \int L(x2 -> x1) * f(x2 -> x1 -> x0) * cos1 * dw1 
//	           = Le(x1 -> x0) 
//					+ \int Le(x2 -> x1) * f(x2 -> x1 -> x0) * cos1 * dw1 
//					+ \int (\int L(x3 -> x2) * f(x3 -> x2 -> x1) * cos2 * dw2) * f(x2 -> x1 -> x0) * cos1 * dw1
Color3 PathTracer::m_RayColor(const Ray& ray) const
{
	HitRecord hit_record;
	Color3 out_color;
	
	// miss
	if (!m_scene.HitHappened(ray, hit_record))
		return Color3(); // defualt color (black)

	// debug: normal shading
	Vector3 normal = hit_record.m_hit_unit_normal;
	return Color3(std::abs(normal.m_x), std::abs(normal.m_y), std::abs(normal.m_z));

	///* first term: Le(x1 -> x0) */
	//if (hit_record.m_material.HasEmission())
	//{
	//	Color3 emit_color(hit_record.m_material.GetEmission());
	//	out_color += emit_color;
	//}

	/* second term: \int le(x2 -> x1) * f(x2 -> x1 -> x0) * cos(\theta) dw */
	// random_point_on_light = pdfarealight(); // uniform p = 1/a && 1/p = a
	// dir = brdf * (n * w) * (n' * w') * light_emmit / r ^ 2 / p; (integral on da)


	/* sampling exept light */
	// random_wi_on_hemisphere = pdfHemisphere(); // uniform p = 1 / 4pi?? 1/p = 4pi??
	// indir = brdf * (n * w) * m_RayColor(random_wi_on_hemisphere) / p  (integral on dwi)

	return out_color;
}
