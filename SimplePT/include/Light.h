#pragma once

#include <vector>
#include <string>
#include "Vector.h"
#include "MyMath.h"


struct XmlLightInfo
{
	std::string m_light_mtl_name;
	Vector3 m_radiance;
};


struct LightPrimitiveInfo
{
	LightPrimitiveInfo(unsigned int actor_id, unsigned int primitive_id, double area)
		: m_actor_id(actor_id), m_actor_primitive_id(primitive_id), m_area(area)
	{}
	unsigned int m_actor_id = 0;
	unsigned int m_actor_primitive_id = 0;
	double m_area = -1.0;
};

class SceneLightInfo
{
public:
	inline void AddLightPrimitive(unsigned int actor_id, unsigned int actor_primitive_id, double area)
	{
		m_light_primitives.push_back(LightPrimitiveInfo(actor_id, actor_primitive_id, area));
		m_total_are += area;
		m_accumulated_area.push_back(m_total_are);
	}
	inline double GetTotalArea() const { return m_total_are; }
	LightPrimitiveInfo GetRandomLightByArea(double& get_light_pdf) const 
	{
		double rand_area = SimplePT::GetRandomDouble_min_to_max(0.0, m_total_are);
		auto it = std::upper_bound(m_accumulated_area.begin(), m_accumulated_area.end(), rand_area);
		size_t offset = std::distance(m_accumulated_area.begin(), it);
		get_light_pdf = m_light_primitives[offset].m_area / m_total_are;
		return m_light_primitives[offset];
	}

private:
	double m_total_are = 0.0;
	std::vector<double> m_accumulated_area;
	std::vector<LightPrimitiveInfo> m_light_primitives;

};

