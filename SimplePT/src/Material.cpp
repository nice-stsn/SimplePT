#include "Material/Material.h"
#include "MyMath.h"

Material::Material(const tinyobj::material_t& material)
{
	m_kd = Vector3(material.diffuse[0], material.diffuse[1], material.diffuse[2]);
	m_emission = Vector3(material.emission[0], material.emission[1], material.emission[2]);
	m_has_emission = (m_emission.Length() > SimplePT::EPSILON);

	// todo: ...

}
