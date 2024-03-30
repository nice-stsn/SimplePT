#pragma once

#include "Ray.h"
#include "Color.h"
#include "MyMath.h"
#include "Material/Material.h"
#include "Light.h"


struct HitRecord
{
	HitRecord() = default;
	Position3 m_hit_position;
	Vector3 m_hit_unit_normal;

	Material m_material;

	unsigned int dbg_face_id_dir = 0;
	unsigned int dbg_face_id_indir = 0;

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

class HittableBase_WithLight : public HittableBase
{
public:
	virtual void ExtractLightInfo(unsigned int actor_id, SceneLightInfo& out_info) const = 0;
	virtual void SampleLight_ByPrimitiveID(unsigned int primitive_id, HitRecord& out_sample_info, double& local_pdf) const = 0;

private:

};

