#pragma once

#include "Hittable/Hittable.h"
#include "tiny_obj_loader.h"
#include <memory>

struct VertexAttribs
{
	Position3 position;
	Vector3 normal;
	// todo: texture, Vector2
	
};

struct TriangleVid
{
	tinyobj::index_t vid0, vid1, vid2;
};

class Mesh : public HittableBase
{
public:
	Mesh() = default;
	Mesh(const std::string& filename, const std::string& mtl_basepath);  
	virtual ~Mesh() {}
	virtual bool HitHappened(const Ray& ray, double t_min, double t_max, HitRecord& hit_record) const override;

private:
	tinyobj::attrib_t m_attrib;
	std::vector<tinyobj::shape_t> m_shapes; // hittables with mesh
	std::vector<tinyobj::material_t> m_materials; // todo

	bool m_HitTriangle(const Ray& ray, const Position3& v0, const Position3& v1, const Position3& v2, double& t) const;
	void m_GetFace(unsigned int f_id, TriangleVid& out_tri) const;
	void m_GetVertex(const tinyobj::index_t& v_id, VertexAttribs& out_vertex) const;

	unsigned int m_num_vtxs;
	unsigned int m_num_tris;
	
};

