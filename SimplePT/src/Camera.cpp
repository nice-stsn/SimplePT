#include "Camera.h"

//- *eye*:  The camera position is in world space. 
//- *lookat*: The target position in world space that the camera points to. 
//- *up*: The up axis of camera in world space. 
//- *fovy*: The field-of-view on the y-axis of camera, the unit is **degree**.
//- *width/height*: The resolution of camera film and result image.
Camera::Camera(Position3 eye, Position3 lookat, Vector3 up, double fov, unsigned int width, unsigned int height)
	: m_eye(eye), m_lookat(lookat), m_up(up), m_fov(fov), m_width(width), m_height(height)
{
	// compute: TODO
}

Ray Camera::CastRay(unsigned int x_id, unsigned int y_id)
{
	// todo
	return Ray();
}
