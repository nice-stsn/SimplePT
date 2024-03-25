#include "Scene.h"
#include "MyMath.h"


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
