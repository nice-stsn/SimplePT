#pragma once

#include "Vector.h"
#include "Hittable/Hittable.h"
#include "Material/Material.h"
#include "tiny_obj_loader.h"
#include "Light.h"
#include "Material/Texutre.h"
#include <memory>
#include <map>

struct VertexAttribs
{
	Position3 position;
	Vector3 normal;
	double tex_u = 0.0;
	double tex_v = 0.0;
	
};

struct Triangle_info
{
	tinyobj::index_t vid0, vid1, vid2;
	Material tri_material;
};

class MeshBVH;

class Mesh : public HittableBase_WithLight
{
public:
	Mesh() = default;
	Mesh(const std::string& filename, const std::string& mtl_basepath, const std::vector<XmlLightInfo>& lights_info);  
	virtual ~Mesh() override;
	virtual bool HitHappened(const Ray& ray, HitRecord& out_hit_record, double t_min = SimplePT::EPSILON, double t_max = SimplePT::INF) const override;
	virtual void ExtractLightInfo(unsigned int actor_id, SceneLightInfo& out_info) const override;
	virtual void SampleLight_ByPrimitiveID(unsigned int primitive_id, HitRecord& out_sample_info, double& local_pdf) const override;
	bool Triangle_HitHappened(unsigned int tri_id, const Ray& ray, HitRecord& out_hit_record, double t_min = SimplePT::EPSILON, double t_max = SimplePT::INF) const;

	inline unsigned int GetNumTris() const { return m_num_tris; }
	Vector3 GetTriCenter(unsigned int tri_index) const;
	void GetVertexsPosition(unsigned int f_id, Position3& v0, Position3& v1, Position3& v2) const;


private:
	std::vector<XmlLightInfo> m_lights_info;

	tinyobj::attrib_t m_attrib;
	std::vector<tinyobj::shape_t> m_shapes; // hittables with mesh
	std::vector<tinyobj::material_t> m_materials; 
	std::map <std::string, std::unique_ptr<Texture>> m_textures_lookup;
	MeshBVH* m_ptr_bvh = nullptr;

	bool m_Moller_Trumbore(const Ray& ray,
		const Position3& v0, const Position3& v1, const Position3& v2,
		double& coord0, double& coord1, double& coord2,
		double& t_out) const;
	void m_GetFaceVertexAndMaterial(unsigned int f_id, Triangle_info& out_tri) const;
	void m_GetVertex(const tinyobj::index_t& v_id, VertexAttribs& out_vertex) const;
	double m_PrimitiveArea(unsigned int primitive_id) const;

	unsigned int m_num_vtxs = 0;
	unsigned int m_num_tris = 0;
	
};

