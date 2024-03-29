#include "Scene.h"
#include "MyMath.h"
#include <cassert>


Scene::~Scene()
{
}

bool Scene::HitHappened(const Ray& ray, HitRecord& out_hit_record, double t_min, double t_max) const 
{
	double t_near = SimplePT::INF;
	bool b_hit_happened = false;
	HitRecord local_record;

	// iterate over all actors (No BVH): O(N)
	for (std::shared_ptr<HittableBase> actor : m_actors)
	{
		// compare & update
		if (actor->HitHappened(ray, local_record, t_min, t_near) && local_record.m_t < t_near)
		{
			b_hit_happened = true;
			t_near = local_record.m_t;
			out_hit_record = local_record;
		}
	}
	
	return b_hit_happened;
}

void Scene::SampleLight(HitRecord& sample_info, double& pdf) const
{
	// assert only one mesh actor in scene
	assert(m_actors.size() == 1);

	//m_actors[0].SampleLight(sample_info, pdf);
	// todo: temp for cornell box uniform sampling 
	double x0 = 213, x1 = 343;
	double z0 = 227, z1 = 332;
	pdf = 1 / ((x1 - x0) * (z1 - z0)); // Uniform light

	double rand_x = SimplePT::GetRandomDouble_min_to_max(x0, x1);
	double rand_z = SimplePT::GetRandomDouble_min_to_max(z0, z1);
	sample_info.m_hit_position = Position3(rand_x, 548.799, rand_z); // cornell box light
	sample_info.m_hit_unit_normal = Vector3(0.0, -1.0, 0.0); // cornell box light
	//sample_info.m_material = Material(Vector3(0.8, 0.8, 0.8)); // temp
	sample_info.m_material = Material(Vector3(34.0, 24.0, 8.0)); // temp radiance

}


