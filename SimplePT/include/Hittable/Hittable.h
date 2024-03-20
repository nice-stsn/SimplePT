#pragma once

#include "Ray.h"


class HitRecord
{
public:


private:


};

class HittableBase
{
public:
	HittableBase() {}
	virtual ~HittableBase() = 0;
	virtual bool Hit(const Ray& ray, double t_min, double t_max, HitRecord& hit_record) = 0;
private:

};

