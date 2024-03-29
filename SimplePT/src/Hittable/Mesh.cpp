#include "Hittable/MeshBVH.h"
#include "Hittable/Mesh.h"
#include "MyMath.h"
#include "tiny_obj_loader.h"
#include <vector>
#include <iostream>
#include <cassert>

Mesh::Mesh(const std::string& filename, const std::string& mtl_basepath, 
	const std::vector<XmlLightInfo>& lights_info)
	: m_lights_info(std::move(lights_info))
{
	// use tiny_obj_loader: https://github.com/tinyobjloader/tinyobjloader/blob/release/loader_example.cc
	bool triangulate = true; // how do i know? todo.
	std::string warn;
	std::string err;

	std::clog << "Start loading obj...\nPlease wait..." << std::endl;
	bool ret = tinyobj::LoadObj(&m_attrib, &m_shapes, &m_materials, &warn, &err,
		filename.c_str(), mtl_basepath.c_str(), triangulate);

	if (!warn.empty())
	{
		std::clog << "WARN: " << warn << std::endl;
	}
	if (!err.empty())
	{
		std::cerr << "ERROR: " << err << std::endl;
	}
	if (!ret)
	{
		std::cerr << "Failed to load: " << filename << std::endl;
		return;
	}
	tinyobj::PrintInfo(m_attrib, m_shapes, m_materials);
	std::cerr << "Load sucess." << std::endl;
	// end tinyobj load

	// set light materials
	for (auto& material : m_materials)
	{
		for (auto& light_info : m_lights_info)
		{
			if (material.name == light_info.m_light_mtl_name)
			{
				material.emission[0] = static_cast<tinyobj::real_t>(light_info.m_radiance.m_x);
				material.emission[1] = static_cast<tinyobj::real_t>(light_info.m_radiance.m_y);
				material.emission[2] = static_cast<tinyobj::real_t>(light_info.m_radiance.m_z);
			}
		}
	}

	m_num_vtxs = static_cast<int>(m_attrib.vertices.size()) / 3;
	m_num_tris = 0;
	for (size_t i = 0; i < m_shapes.size(); ++i)
	{
		m_num_tris += static_cast<unsigned int>(m_shapes[i].mesh.num_face_vertices.size());
	}

	// build bvh
	assert(m_shapes.size() == 1);
	m_ptr_bvh = new MeshBVH(this);
	std::clog << "Start building bvh...\nPlease wait..." << std::endl;
	m_ptr_bvh->BuildBVH();
	std::clog << "Build sucess." << std::endl;
}

Mesh::~Mesh()
{
	delete m_ptr_bvh;
}

void Mesh::m_GetFaceInfo(unsigned int f_id, Triangle_info& out_tri) const
{
	// for scnens in 'example-scenes-cg23', shape.size() == 1
	assert(m_shapes.size() == 1);

	const tinyobj::mesh_t& mesh = m_shapes[0].mesh;
	int num_face = m_shapes[0].mesh.num_face_vertices[0]; // 3 is triangle, 4 is quad ..
	out_tri.vid0 = mesh.indices[f_id * num_face + 0];
	out_tri.vid1 = mesh.indices[f_id * num_face + 1];
	out_tri.vid2 = mesh.indices[f_id * num_face + 2];

	int material_id = mesh.material_ids[f_id];
	out_tri.tri_material = Material(m_materials[material_id]);

}


void Mesh::m_GetVertex(const tinyobj::index_t& v_id, VertexAttribs& out_vertex) const
{
	double px = m_attrib.vertices[v_id.vertex_index * 3 + 0];
	double py = m_attrib.vertices[v_id.vertex_index * 3 + 1];
	double pz = m_attrib.vertices[v_id.vertex_index * 3 + 2];

	double nx = m_attrib.normals[v_id.vertex_index * 3 + 0];
	double ny = m_attrib.normals[v_id.vertex_index * 3 + 1];
	double nz = m_attrib.normals[v_id.vertex_index * 3 + 2];

	out_vertex.position = Position3(px, py, pz);
	out_vertex.normal = Vector3(nx, ny, nz);
	// todo: texture

}



bool Mesh::HitHappened(const Ray& ray, HitRecord& out_hit_record, double t_min, double t_max) const 
{
	// bvh
	if (m_ptr_bvh)
		return m_ptr_bvh->HitHappened(ray, out_hit_record, t_min, t_max);
	else
	{
		std::clog << "BVH of mesh is not built." << std::endl;
		return false;
	}
}

bool Mesh::Triangle_HitHappened(unsigned int tri_id, const Ray& ray, HitRecord& out_hit_record, double t_min, double t_max) const
{
	Triangle_info tri;
	m_GetFaceInfo(tri_id, tri);

	VertexAttribs v0, v1, v2;
	m_GetVertex(tri.vid0, v0);
	m_GetVertex(tri.vid1, v1);
	m_GetVertex(tri.vid2, v2);

	double t_triangle = -1;
	if (m_HitTriangle(ray, v0.position, v1.position, v2.position, t_triangle) && t_triangle > t_min && t_triangle < t_max)
	{
		out_hit_record.m_t = t_triangle; // record smaller

		// flat normal
		Vector3 avg_normal = ((v0.normal + v1.normal + v2.normal) / 3).Normalized();
		out_hit_record.m_hit_position = ray.GetOrigin() + t_triangle * ray.GetDirection();
		out_hit_record.m_hit_unit_normal = avg_normal; 
		out_hit_record.m_material = tri.tri_material;

		// debug info
		out_hit_record.dbg_face_id_dir = tri_id;

		return true;
	}

	return false;
}

bool Mesh::m_HitTriangle(const Ray& ray, const Position3& v0, const Position3& v1, const Position3& v2, double& t_out) const
{
	// Moller-Trumbore Algorithm https://zhuanlan.zhihu.com/p/451582864
	Vector3 S = ray.GetOrigin() - v0;
	Vector3 d = ray.GetDirection();
	Vector3 E1 = v1 - v0;
	Vector3 E2 = v2 - v0;
	Vector3 S1 = CrossProduct(d, E2);
	Vector3 S2 = CrossProduct(S, E1);

	double S1E1 = DotProduct(S1, E1);
	if (SimplePT::Equal(S1E1, 0))
	{
		// eye ray is in the plane of triangle
		return false;
	}
	double t = DotProduct(S2, E2) / S1E1;
	double b1 = DotProduct(S1, S) / S1E1;
	double b2 = DotProduct(d, S2) / S1E1;

	if (t >= SimplePT::EPSILON && b1 >= 0.0f && b2 >= 0.0f && (1 - b1 - b2) >= 0.0f)
	{
		t_out = t;

		/* future texture */
		// u = b1;
		// v = b2;
		return true;
	}
	return false;

}

Vector3 Mesh::GetTriCenter(unsigned int tri_index) const
{
	Triangle_info tri;
	m_GetFaceInfo(tri_index, tri);

	VertexAttribs v0, v1, v2;
	m_GetVertex(tri.vid0, v0);
	m_GetVertex(tri.vid1, v1);
	m_GetVertex(tri.vid2, v2);

	return (v0.position + v1.position + v2.position) * 0.3333333;
}


void Mesh::GetVertexsPosition(unsigned int f_id, Position3& v0, Position3& v1, Position3& v2) const
{
	Triangle_info tri;
	m_GetFaceInfo(f_id, tri);
	VertexAttribs va0, va1, va2;
	m_GetVertex(tri.vid0, va0);
	m_GetVertex(tri.vid1, va1);
	m_GetVertex(tri.vid2, va2);
	v0 = va0.position;
	v1 = va1.position;
	v2 = va2.position;
}


void Mesh::ExtractLightInfo(unsigned int actor_id, SceneLightInfo& out_info) const
{
	for (unsigned int i = 0; i < m_num_tris; ++i)
	{
		Triangle_info tri;
		m_GetFaceInfo(i, tri);

		if (tri.tri_material.HasEmission())
		{
			// compute area
			double area = m_PrimitiveArea(i);
			out_info.AddLightPrimitive(actor_id, i, area);
		}
	}
}

void Mesh::SampleLight_ByPrimitiveID(unsigned int primitive_id, HitRecord& out_sample_info, double& local_pdf) const
{
	// https://kingins.cn/2022/03/08/%E4%B8%89%E8%A7%92%E5%BD%A2%E9%9A%8F%E6%9C%BA%E5%9D%87%E5%8C%80%E7%82%B9%E9%87%87%E6%A0%B7%E7%AE%97%E6%B3%95/
	Triangle_info tri;
	m_GetFaceInfo(primitive_id, tri);
	assert(tri.tri_material.HasEmission());

	VertexAttribs v0, v1, v2;
	m_GetVertex(tri.vid0, v0);
	m_GetVertex(tri.vid1, v1);
	m_GetVertex(tri.vid2, v2);

	double u = SimplePT::GetRandomDouble_0_to_1();
	double sqrt_v = std::sqrt(SimplePT::GetRandomDouble_0_to_1());
	double a = sqrt_v * (1 - u);
	double b = u * sqrt_v;
	double c = 1 - sqrt_v;

	out_sample_info.m_hit_position = a * v0.position + b * v1.position + c * v2.position;
	out_sample_info.m_hit_unit_normal = (a * v0.normal + b * v1.normal + c * v2.normal).Normalized();
	out_sample_info.dbg_face_id_dir = primitive_id;
	out_sample_info.m_material = tri.tri_material;

	local_pdf = m_PrimitiveArea(primitive_id);

}

double Mesh::m_PrimitiveArea(unsigned int primitive_id) const
{
	Position3 A, B, C;
	GetVertexsPosition(primitive_id, A, B, C);
	Vector3 E1 = B - A;
	Vector3 E2 = C - A;
	return 0.5 * CrossProduct(E1, E2).Length();
}
