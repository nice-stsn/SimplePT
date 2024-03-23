#include "Hittable/Mesh.h"
#include "MyMath.h"
#include "tiny_obj_loader.h"
#include <vector>
#include <iostream>
#include <cassert>

Mesh::Mesh(const std::string& filename, const std::string& mtl_basepath)
{
	// use tiny_obj_loader: https://github.com/tinyobjloader/tinyobjloader/blob/release/loader_example.cc
	bool triangulate = true; // ? how do i know
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
		std::cerr << "WARN: " << warn << std::endl;
	}

	if (!ret)
	{
		std::cerr << "Failed to load: " << filename << std::endl;
		return;
	}
	
	tinyobj::PrintInfo(m_attrib, m_shapes, m_materials);
	// end tinyobj load

	m_num_vtxs = static_cast<int>(m_attrib.vertices.size()) / 3;
	m_num_tris = 0;
	for (size_t i = 0; i < m_shapes.size(); ++i)
	{
		m_num_tris += static_cast<unsigned int>(m_shapes[i].mesh.num_face_vertices.size());
	}

}

// single triangle for debugging
Mesh::Mesh(const Vector3& v0, const Vector3& v1, const Vector3& v2)
{
	//m_vertices = std::make_unique<Position3[]>(3);
	//m_triangles = std::make_unique<TriangleVid[]>(1);
	//m_vertices[0] = v0;
	//m_vertices[1] = v1;
	//m_vertices[2] = v2;
	//m_triangles[0].vid0 = 0;
	//m_triangles[0].vid1 = 1;
	//m_triangles[0].vid2 = 2;
	//m_num_tris = 1;
	//m_num_vtxs = 3;
}


void Mesh::m_GetFace(unsigned int f_id, TriangleVid& out_tri) const
{
	// for scnens in 'example-scenes-cg23', shape.size() == 1
	assert(m_shapes.size() == 1);

	const tinyobj::mesh_t& mesh = m_shapes[0].mesh;
	int num_face = m_shapes[0].mesh.num_face_vertices[0]; // 3 is triangle, 4 is quad ..
	out_tri.vid0 = mesh.indices[f_id * num_face + 0];
	out_tri.vid1 = mesh.indices[f_id * num_face + 1];
	out_tri.vid2 = mesh.indices[f_id * num_face + 2];

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
}



bool Mesh::HitHappened(const Ray& ray, double t_min, double t_max, HitRecord& hit_record) const
{
	// bf: iterate over all triangles
	// for scnen in 'example-scenes-cg23', shape.size() == 1
	assert(m_shapes.size() == 1);

	bool b_hit_happened = false;
	for (unsigned int i = 0; i < m_num_tris; ++i)
	{
		TriangleVid tri;
		m_GetFace(i, tri);

		VertexAttribs v0, v1, v2;
		m_GetVertex(tri.vid0, v0);
		m_GetVertex(tri.vid1, v1);
		m_GetVertex(tri.vid2, v2);

		double t_triangle = -1;
		if (m_HitTriangle(ray, v0.position, v1.position, v2.position, t_triangle) && t_triangle > t_min && t_triangle < t_max && t_triangle < hit_record.m_t)
		{
			hit_record.m_t = t_triangle; // record smaller
			b_hit_happened = true;

			// shading with normal
			Vector3 avg_normal = ((v0.normal + v1.normal + v2.normal) / 3).Normalized();
			//hit_record.m_color = Color3(255u, 0u, 0u); // RED
			hit_record.m_color = Color3(avg_normal.m_x, avg_normal.m_y, avg_normal.m_z); // normal

		}
	}

	return b_hit_happened;
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
