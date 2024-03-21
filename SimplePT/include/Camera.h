#pragma once

#include "Vector.h"
#include "Ray.h"

class Camera
{
public:
	Camera() = default;
	Camera(const Position3& eye, const Position3& lookat, const Vector3& up, double fovy, unsigned int width, unsigned int height);
	~Camera() {}

	inline unsigned int GetWidth() const { return m_image_width; }
	inline unsigned int GetHeight() const { return m_image_height; }

	Ray CastRay(unsigned int x_id, unsigned int y_id) const;
	
private:
	Position3 m_eye, m_lookat;
	Vector3 m_up;
	double m_fovy;
	unsigned int m_image_width, m_image_height;

	Vector3 m_delta_r_per_pixel;  // left to right
	Vector3 m_delta_u_per_pixel;  // top  to bottom
	Position3 m_center_of_upper_left_pixel;
};

