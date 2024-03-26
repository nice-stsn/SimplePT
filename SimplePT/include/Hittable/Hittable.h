#pragma once

#include "Ray.h"
#include "Color.h"
#include "MyMath.h"
#include "Material/Material.h"


struct HitRecord
{
	HitRecord() = default;
	Color3 m_color; // todo: delete this, color should be compute with all record member
	Position3 m_hit_position;
	Vector3 m_hit_unit_normal;

	Material m_material;

	unsigned int dbg_face_id_dir;
	unsigned int dbg_face_id_indir;

	double m_t = SimplePT::INF;

};

class HittableBase
{
public:
	HittableBase() {}
	virtual ~HittableBase() = 0;
	virtual bool HitHappened(const Ray& ray, HitRecord& out_hit_record, double t_min = SimplePT::EPSILON, double t_max = SimplePT::INF) const = 0;
private:

};

