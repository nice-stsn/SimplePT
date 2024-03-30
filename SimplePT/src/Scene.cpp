#include "Scene.h"
#include "MyMath.h"
#include <cassert>
#include <algorithm>


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

void Scene::ComputeLightInfo()
{
	assert(m_actors.size() == 1); // true in examples; need to loop for actor if needed

	for (unsigned int actor_id = 0; actor_id < m_actors.size(); ++actor_id)
	{
		const HittableBase_WithLight* actor = m_actors[actor_id].get();
		assert(actor != nullptr);
		actor->ExtractLightInfo(actor_id, m_light_list);
	}

}

void Scene::SampleLight(HitRecord& sample_info, double& pdf) const
{
	// assert only one mesh actor in scene
	assert(m_actors.size() == 1);

	/* sample lights */
	double get_light_pdf = -1.0;
	double local_pdf = -1.0;

	//LightPrimitiveInfo light_primitive = m_light_list.GetRandomLightByArea(get_light_pdf);
	LightPrimitiveInfo light_primitive = m_light_list.GetRandomLightByAreaRadiance(get_light_pdf);
	auto& actor = m_actors[light_primitive.m_actor_id];
	actor->SampleLight_ByPrimitiveID(light_primitive.m_actor_primitive_id, sample_info, local_pdf); // assert emission

	pdf = get_light_pdf * local_pdf;
	assert(local_pdf <= 1.0 && local_pdf >= 0.0);
	assert(pdf <= 1.0 && pdf >= 0.0);

}


