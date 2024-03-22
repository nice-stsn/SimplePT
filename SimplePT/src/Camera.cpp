#include "Camera.h"
#include "MyMath.h"

//- *eye*:  The camera position is in world space. 
//- *lookat*: The target position in world space that the camera points to. 
//- *up*: The up axis of camera in world space. 
//- *fovy*: The field-of-view on the y-axis of camera, the unit is **degree**.
//- *width/height*: The resolution of camera film and result image.
Camera::Camera(const Position3& eye, const Position3& lookat, const Vector3& up, double fovy, unsigned int width, unsigned int height)
	: m_eye(eye), m_lookat(lookat), m_up(up), m_fovy(fovy), m_image_width(width), m_image_height(height)
{
	double focal_len = 1.0f;
	double aspect_ratio = static_cast<double>(m_image_width) / m_image_height;
	
	double half_height = std::tan(static_cast<long double>(SimplePT::deg2rad(m_fovy / 2))) * focal_len;
	double half_width = half_height * aspect_ratio;

	// Camera local coordinate
	// https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/lookat-function/framing-lookat-function.html
	// F(unit): point from lookat to eye (point to foward)
	// R(unit): point to right relative to camera
	// U(unit): point to up relative to camera
	Vector3 F = (m_eye - m_lookat).Normalized();
	Vector3 R = CrossProduct(m_up, F).Normalized();
	Vector3 U = CrossProduct(F, R).Normalized();

	double pixel_width = 2 * half_width / m_image_width;     // left to right
	double pixel_height = 2 * half_height / m_image_height;  // top  to bottom

	m_delta_r_per_pixel =  pixel_height * R;
	m_delta_u_per_pixel = -pixel_height * U;
	m_center_of_upper_left_pixel = m_eye - focal_len * F 
		+ half_height * U - half_width * R
		+ m_delta_r_per_pixel / 2 + m_delta_u_per_pixel / 2;
}

Ray Camera::CastRay(unsigned int x_id, unsigned int y_id) const
{
	Position3 pixel_center_i_j = 
		m_center_of_upper_left_pixel + m_delta_r_per_pixel * x_id + m_delta_u_per_pixel * y_id;
	Vector3 direction = pixel_center_i_j - m_eye;

	return Ray(m_eye, direction);
}
