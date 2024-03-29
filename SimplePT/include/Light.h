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
	LightPrimitiveInfo(unsigned int actor_id, unsigned int primitive_id, double area, double area_radiance)
		: m_actor_id(actor_id), m_actor_primitive_id(primitive_id), m_area(area), m_area_radiance(area_radiance)
	{
	}
	unsigned int m_actor_id = 0;
	unsigned int m_actor_primitive_id = 0;
	double m_area = -1.0;
	double m_area_radiance = -1.0;
};

class SceneLightInfo
{
public:
	inline void AddLightPrimitive(unsigned int actor_id, unsigned int actor_primitive_id, double area, const Vector3& radiance)
	{
		double max_radiance = 0.0;
		max_radiance = (radiance.m_x > max_radiance) ? radiance.m_x : max_radiance;
		max_radiance = (radiance.m_y > max_radiance) ? radiance.m_y : max_radiance;
		max_radiance = (radiance.m_z > max_radiance) ? radiance.m_z : max_radiance;
		double area_radiance = area * max_radiance;

		m_light_primitives.push_back(LightPrimitiveInfo(actor_id, actor_primitive_id, area, area_radiance));
		m_total_area += area;
		m_total_area_radiance += area_radiance;
		m_accumulated_area.push_back(m_total_area);
		m_accumulated_area_radiance.push_back(m_total_area_radiance);
	}
	inline double GetTotalArea() const { return m_total_area; }
	LightPrimitiveInfo GetRandomLightByArea(double& get_light_pdf) const 
	{
		double rand_area = SimplePT::GetRandomDouble_min_to_max(0.0, m_total_area);
		auto it = std::upper_bound(m_accumulated_area.begin(), m_accumulated_area.end(), rand_area);
		size_t offset = std::distance(m_accumulated_area.begin(), it);
		get_light_pdf = m_light_primitives[offset].m_area / m_total_area;
		return m_light_primitives[offset];
	}

	LightPrimitiveInfo GetRandomLightByAreaRadiance(double& get_light_pdf) const 
	{
		double rand_area_radiance = SimplePT::GetRandomDouble_min_to_max(0.0, m_total_area_radiance);
		auto it = std::upper_bound(m_accumulated_area_radiance.begin(), m_accumulated_area_radiance.end(), rand_area_radiance);
		size_t offset = std::distance(m_accumulated_area_radiance.begin(), it);
		get_light_pdf = m_light_primitives[offset].m_area_radiance / m_total_area_radiance;
		return m_light_primitives[offset];
	}


private:
	double m_total_area = 0.0;
	double m_total_area_radiance = 0.0;
	std::vector<double> m_accumulated_area;
	std::vector<double> m_accumulated_area_radiance;
	std::vector<LightPrimitiveInfo> m_light_primitives;

};

