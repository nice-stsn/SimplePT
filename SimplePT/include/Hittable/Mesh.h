#pragma once

#include "Vector.h"
#include "Hittable/Hittable.h"
#include "Material/Material.h"
#include "tiny_obj_loader.h"
#include "Light.h"
#include <memory>

struct VertexAttribs
{
	Position3 position;
	Vector3 normal;
	// todo: texture, Vector2
	
};

struct Triangle_info
{
	tinyobj::index_t vid0, vid1, vid2;
	Material tri_material;
};

class MeshBVH;

class Mesh : public HittableBase
{
public:
	Mesh() = default;
	Mesh(const std::string& filename, const std::string& mtl_basepath, const std::vector<LightInfo>& lights_info);  
	virtual ~Mesh();
	virtual bool HitHappened(const Ray& ray, HitRecord& out_hit_record, double t_min = SimplePT::EPSILON, double t_max = SimplePT::INF) const override;
	bool Triangle_HitHappened(unsigned int tri_id, const Ray& ray, HitRecord& out_hit_record, double t_min = SimplePT::EPSILON, double t_max = SimplePT::INF) const;

	inline unsigned int GetNumTris() const { return m_num_tris; }
	Vector3 GetTriCenter(unsigned int tri_index) const;
	void GetVertexsPosition(unsigned int f_id, Position3& v0, Position3& v1, Position3& v2) const;

private:
	std::vector<LightInfo> m_lights_info;

	tinyobj::attrib_t m_attrib;
	std::vector<tinyobj::shape_t> m_shapes; // hittables with mesh
	std::vector<tinyobj::material_t> m_materials; 
	MeshBVH* m_ptr_bvh = nullptr;

	bool m_HitTriangle(const Ray& ray, const Position3& v0, const Position3& v1, const Position3& v2, double& t) const;
	void m_GetFaceInfo(unsigned int f_id, Triangle_info& out_tri) const;
	void m_GetVertex(const tinyobj::index_t& v_id, VertexAttribs& out_vertex) const;

	unsigned int m_num_vtxs = 0;
	unsigned int m_num_tris = 0;
	
};

