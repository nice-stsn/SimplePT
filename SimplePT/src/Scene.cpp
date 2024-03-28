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
	// todo
	//pdf = SimplePT::GetRandomDouble_0_to_1();
	pdf = 0.5;
	sample_info.m_hit_position = Position3(300.0, 548.799, 300.0); // cornell box light
	sample_info.m_hit_unit_normal = Vector3(0.0, -1.0, 0.0); // cornell box light
	sample_info.m_material = Material(Vector3(0.8, 0.8, 0.8));

}


