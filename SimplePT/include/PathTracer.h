#pragma once

#include "Scene.h"
#include "Camera.h"

class PathTracer
{
public:
	PathTracer(Scene scn, Camera cam) : m_scn(scn), m_cam(cam) { }

	void Render(int num_samples_per_pixel = 1);



private:
	Scene& m_scn;
	Camera& m_cam;

};
