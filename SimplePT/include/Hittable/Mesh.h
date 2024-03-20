#pragma once

#include "Hittable/Hittable.h"

class Mesh : public HittableBase
{
public:
	Mesh() {}
	virtual ~Mesh() {}
	virtual bool Hit(const Ray& ray, double t_min, double t_max, HitRecord& hit_record) override;

private:

	
};
