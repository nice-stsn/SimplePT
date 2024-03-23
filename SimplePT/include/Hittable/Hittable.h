#pragma once

#include "Ray.h"
#include "Color.h"
#include "MyMath.h"


struct HitRecord
{
	Color3 m_color;
	Position3 m_hit_position;
	Position3 m_hit_unit_normal;

	double m_t = SimplePT::INF;

};

class HittableBase
{
public:
	HittableBase() {}
	virtual ~HittableBase() = 0;
	virtual bool HitHappened(const Ray& ray, double t_min, double t_max, HitRecord& hit_record) const = 0;
private:

};

