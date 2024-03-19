#pragma once

#include "Vector.h"

class Camera
{
public:
	Camera() = default;
	Camera(Position3 eye, Position3 lookat, Vector3 up, double fov, unsigned int width, unsigned int height);
	~Camera() {}

	inline unsigned int GetWidth() const { return m_width; }
	inline unsigned int GetHeight() const { return m_height; }

private:
	Position3 m_eye, m_lookat;
	Vector3 m_up;
	double m_fov;
	unsigned m_width, m_height;
};

