#pragma once
#include "Hittable/BVHBase.h"

class Mesh;
class MeshBVH : public BVHBase
{
public:
	MeshBVH(const Mesh* ptr_mesh) : m_ptr_mesh(ptr_mesh) {}
	~MeshBVH() {}
	virtual void ExtractLightInfo(unsigned int actor_id, SceneLightInfo& out_info) const override {}
	virtual void SampleLight_ByPrimitiveID(unsigned int primitive_id, HitRecord& out_sample_info, double& local_pdf) const override{}
private:
	const Mesh* m_ptr_mesh;

	virtual bool m_Primitive_HitHappend(unsigned int node_index, const Ray& ray, HitRecord& out_hit_record, double t_min, double t_max) const override;
	virtual Vector3 m_GetPrimitiveCenter(unsigned int primitive_index) const override;
	virtual unsigned int m_GetPrimitiveNumber() const override;
	virtual void m_InitPrimitiveAABBs() override;

};
