#include "Hittable/Mesh.h"
#include "MyMath.h"
#include <cassert>


// single triangle for debugging
Mesh::Mesh(const Vector3& v0, const Vector3& v1, const Vector3& v2)
{
	m_vertices = std::make_unique<Position3[]>(3);
	m_triangles = std::make_unique<TriangleVid[]>(1);
	m_vertices[0] = v0;
	m_vertices[1] = v1;
	m_vertices[2] = v2;
	m_triangles[0].vid0 = 0;
	m_triangles[0].vid1 = 1;
	m_triangles[0].vid2 = 2;
	m_num_tris = 1;
	m_num_vtxs = 3;
}

bool Mesh::HitHappened(const Ray& ray, double t_min, double t_max, HitRecord& hit_record) const
{
	// todo
	// bf: iterate over all triangles
	bool b_hit_happened = false;
	for (unsigned int i = 0; i < m_num_tris; ++i)
	{
		Position3 v0 = m_vertices[m_triangles[i].vid0];
		Position3 v1 = m_vertices[m_triangles[i].vid1];
		Position3 v2 = m_vertices[m_triangles[i].vid2];
		double t_triangle = -1;
		if (m_HitTriangle(ray, v0, v1, v2, t_triangle) && t_triangle > t_min && t_triangle < t_max)
		{
			b_hit_happened = true;
			hit_record.m_t = t_triangle;
			hit_record.m_color = Color3(255u, 0u, 0u);
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

	

	//// debug single_triangle
	//// find t
	//Position3 eye = ray.GetOrigin();
	//Vector3 dir = ray.GetDirection();
	//double plane_z = v0.m_z; // assmune v0, v1, v2 at the same plane
	//t = (plane_z - eye.m_z) / dir.m_z;
	//if (t < 0)
	//	return false;

	//// find 
	//// v2
	//// v1   v0
	//double hit_x = eye.m_x + t * dir.m_x;
	//double hit_y = eye.m_y + t * dir.m_y;
	//if (hit_x > v2.m_x && hit_y > v0.m_y && hit_x < v0.m_x && hit_y < v2.m_y)
	//	return true;
	//else 
	//	return false;

}
