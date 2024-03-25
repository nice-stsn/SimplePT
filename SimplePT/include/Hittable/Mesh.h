#pragma once

#include "Hittable/Hittable.h"
#include "Material/Material.h"
#include "tiny_obj_loader.h"
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

class Mesh : public HittableBase
{
public:
	Mesh() = default;
	Mesh(const std::string& filename, const std::string& mtl_basepath, const std::string& light_mtl_name, const Vector3& light_radiance);  
	virtual ~Mesh() {}
	virtual bool HitHappened(const Ray& ray, HitRecord& out_hit_record, double t_min = SimplePT::EPSILON, double t_max = SimplePT::INF) const override;

private:
	std::string m_light_mtlname;
	Vector3 m_radiance;

	tinyobj::attrib_t m_attrib;
	std::vector<tinyobj::shape_t> m_shapes; // hittables with mesh
	std::vector<tinyobj::material_t> m_materials; // todo


	bool m_HitTriangle(const Ray& ray, const Position3& v0, const Position3& v1, const Position3& v2, double& t) const;
	void m_GetFaceInfo(unsigned int f_id, Triangle_info& out_tri) const;
	void m_GetVertex(const tinyobj::index_t& v_id, VertexAttribs& out_vertex) const;

	unsigned int m_num_vtxs = 0;
	unsigned int m_num_tris = 0;
	
};

