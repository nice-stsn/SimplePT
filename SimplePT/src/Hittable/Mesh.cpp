#include "Hittable/MeshBVH.h"
#include "Hittable/Mesh.h"
#include "MyMath.h"
#include "tiny_obj_loader.h"
#include <vector>
#include <iostream>
#include <cassert>

Mesh::Mesh(const std::string& filename, const std::string& mtl_basepath, const std::string& light_mtl_name, const Vector3& light_radiance)
	: m_light_mtlname(light_mtl_name), m_radiance(light_radiance)
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
	// end tinyobj load

	// set light materials
	for (auto& material : m_materials)
	{
		if (material.name == m_light_mtlname)
		{
			material.emission[0] = static_cast<tinyobj::real_t>(m_radiance.m_x);
			material.emission[1] = static_cast<tinyobj::real_t>(m_radiance.m_y);
			material.emission[2] = static_cast<tinyobj::real_t>(m_radiance.m_z);
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
	m_ptr_bvh->BuildBVH();
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

	// bf: iterate over all triangles
	//{
	//	// for scnen in 'example-scenes-cg23', shape.size() == 1
	//	assert(m_shapes.size() == 1);

	//	bool b_hit_happened = false;
	//	for (unsigned int i = 0; i < m_num_tris; ++i)
	//	{
	//		Triangle_info tri;
	//		m_GetFaceInfo(i, tri);

	//		VertexAttribs v0, v1, v2;
	//		m_GetVertex(tri.vid0, v0);
	//		m_GetVertex(tri.vid1, v1);
	//		m_GetVertex(tri.vid2, v2);

	//		double t_triangle = -1;
	//		if (m_HitTriangle(ray, v0.position, v1.position, v2.position, t_triangle) && t_triangle > t_min && t_triangle < t_max && t_triangle < out_hit_record.m_t)
	//		{
	//			out_hit_record.m_t = t_triangle; // record smaller
	//			b_hit_happened = true;

	//			// shading with normal
	//			Vector3 avg_normal = ((v0.normal + v1.normal + v2.normal) / 3).Normalized();
	//			out_hit_record.m_hit_unit_normal = avg_normal; // flat normal
	//			out_hit_record.m_material = tri.tri_material;
	//		}
	//	}

	//	return b_hit_happened;
	//}
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

		// shading with normal
		Vector3 avg_normal = ((v0.normal + v1.normal + v2.normal) / 3).Normalized();
		out_hit_record.m_hit_unit_normal = avg_normal; // flat normal
		out_hit_record.m_material = tri.tri_material;
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


