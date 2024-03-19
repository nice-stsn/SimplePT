#pragma once

#include <memory>
#include "Scene.h"
#include "Camera.h"
#include "Color.h"


class PathTracer
{
public:
	PathTracer(const Scene& scn, const Camera& cam) : m_scn(scn), m_cam(cam) {
		m_frame_buffer 
			= std::make_unique<unsigned char[]>(cam.GetWidth() * cam.GetHeight() * CHANNEL_NUM);
	}


	void Render(int num_samples_per_pixel = 1);



private:
	static const int CHANNEL_NUM = 3;
	const Scene& m_scn;
	const Camera& m_cam;
	std::unique_ptr<unsigned char[]> m_frame_buffer;

	bool WritePixelColor(unsigned int x_id, unsigned int y_id, const Color3& col);

};
