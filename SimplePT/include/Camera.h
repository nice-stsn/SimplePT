#pragma once

#include "Vector.h"

class Camera
{
public:
	Camera() = default;
	Camera(Position3 eye, Position3 lookat, Vector3 up, double fov, unsigned int width, unsigned int height);
	~Camera() {}

private:
	Position3 m_eye, m_lookat;
	Vector3 m_up;
	double m_fov;
	unsigned m_width, m_height;
};

