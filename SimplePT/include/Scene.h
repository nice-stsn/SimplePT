#pragma once

#include "Hittable/Hittable.h"
#include "Hittable/Mesh.h"
#include <memory>
#include <vector>

class Scene : public HittableBase
{
public:
	Scene() = default;
	virtual ~Scene() override;

	void AddHittableObject(const std::shared_ptr<HittableBase>& actor);
	virtual bool HitHappened(const Ray& ray, HitRecord& out_hit_record, double t_min = SimplePT::EPSILON, double t_max = SimplePT::INF) const override;

	void SampleLight(HitRecord& hit_record, double& pdf) const;

private:
	std::vector<std::shared_ptr<HittableBase>> m_actors;

	// todo: bvh

};

inline void Scene::AddHittableObject(const std::shared_ptr<HittableBase>& actor)
{
	m_actors.push_back(actor);
}
