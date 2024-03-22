#pragma once

#include "Hittable/Hittable.h"
#include <memory>

struct TriangleVid
{
	unsigned int vid0, vid1, vid2;
};

class Mesh : public HittableBase
{
public:
	Mesh() {}
	Mesh(const Position3& v0, const Position3& v1, const Position3& v2);  // single triangle for debugging
	virtual ~Mesh() {}
	virtual bool HitHappened(const Ray& ray, double t_min, double t_max, HitRecord& hit_record) const override;

private:
	std::unique_ptr<Position3[]> m_vertices;
	std::unique_ptr<TriangleVid[]> m_triangles;
	unsigned int m_num_vtxs;
	unsigned int m_num_tris;

	bool m_HitTriangle(const Ray& ray, const Position3& v0, const Position3& v1, const Position3& v2, double& t) const;
	
};
