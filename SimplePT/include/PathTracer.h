#pragma once

#include <memory>
#include "Scene.h"
#include "Camera.h"
#include "Color.h"
#include "Hittable/Hittable.h"


class PathTracer
{
public:
	PathTracer(const Scene& scn, const Camera& cam, double RussianRoulette) 
		: m_scene(scn), m_camera(cam), m_width(0), m_height(0), m_RussianRoulette(RussianRoulette)
	{
		m_frame_buffer 
			= std::make_unique<unsigned char[]>(m_camera.GetWidth() * m_camera.GetHeight() * CHANNEL_NUM);
	}


	void Render(int num_samples_per_pixel = 1, double RussianRoulette = 0.8);

private:
	const Scene& m_scene;   // for simplicity
	const Camera& m_camera; // for simplicity
	unsigned int m_width , m_height;
	double m_RussianRoulette;
	std::unique_ptr<unsigned char[]> m_frame_buffer;

	bool m_WritePixelColor(unsigned int x_id, unsigned int y_id, const Color3& col);
	bool m_WritePixelRadiance(unsigned int x_id, unsigned int y_id, Vector3& radiance);
	Vector3 m_RayRadiance(const Ray& ray) const;
	bool m_Visible(const Position3& pos0, const Vector3& normal0, const Position3& pos1) const;
	Vector3 m_MCInt(const HitRecord& hit_record, const Vector3& unit_wo, const Ray& wi_ray, double pdf) const;

	static const int CHANNEL_NUM = 3;
};
