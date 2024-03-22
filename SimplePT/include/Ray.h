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

	inline Position3 GetOrigin() const { return m_origin; }
	inline Vector3 GetUnitDir() const { return m_dir.Normalized(); }
	inline Vector3 GetDirection() const { return m_dir; }

private:
	Position3 m_origin;
	Vector3 m_dir; // not unit 

};

