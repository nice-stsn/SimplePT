#include "Hittable/MeshBVH.h"
#include "Hittable/Mesh.h"

void MeshBVH::m_InitPrimitiveAABBs() 
{
	for (unsigned int i = 0; i < m_GetPrimitiveNumber(); ++i)
	{
		Position3 v0, v1, v2;
		m_ptr_mesh->GetVertexsPosition(i, v0, v1, v2);
		m_primitive_aabbs[i] += v0;
		m_primitive_aabbs[i] += v1;
		m_primitive_aabbs[i] += v2;
	}
}

bool MeshBVH::m_Primitive_HitHappend(unsigned int primiti_index, const Ray& ray, HitRecord& out_hit_record, double t_min, double t_max) const 
{
	return m_ptr_mesh->Triangle_HitHappened(primiti_index, ray, out_hit_record, t_min, t_max);
}

Vector3 MeshBVH::m_GetPrimitiveCenter(unsigned int primitive_index) const 
{
	return m_ptr_mesh->GetTriCenter(primitive_index);
}

unsigned int MeshBVH::m_GetPrimitiveNumber() const 
{
	return m_ptr_mesh->GetNumTris();
}

