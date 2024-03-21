#pragma once

#include "Vector.h"

class Ray
{
public:
	Ray() : m_origin(), m_dir(Vector3(1.0f, 0.0f, 0.0f)) {}
	Ray(const Position3& origin, const Vector3& direction) : m_origin(origin), m_dir(direction) {}

	Position3 At(double t) const { 
		return m_origin + m_dir * t;
	}

private:
	Position3 m_origin;
	Vector3 m_dir; // not unit 

};

