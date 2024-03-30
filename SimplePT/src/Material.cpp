#include "Material/Material.h"
#include "MyMath.h"
#include "tiny_obj_loader.h"

static inline Vector3 TinyobjVectoVector3(const tinyobj::real_t* vec3)
{
	return Vector3(vec3[0], vec3[1], vec3[2]);
}

Material::Material(const tinyobj::material_t& material)
{
	/* material types */
	//real_t ambient[3];
	//real_t diffuse[3];
	//real_t specular[3];
	//real_t transmittance[3];
	//real_t emission[3];
	//real_t shininess;
	//real_t ior;       // index of refraction
	//real_t dissolve;  // 1 == opaque; 0 == fully transparent
	/*=---------------*/

	m_kd = TinyobjVectoVector3(material.diffuse);
	m_ks = TinyobjVectoVector3(material.specular);
	m_tr = TinyobjVectoVector3(material.transmittance);
	m_ns = material.shininess;
	m_ior = material.ior;
	m_emission = Vector3(material.emission[0], material.emission[1], material.emission[2]);
	m_has_emission = (m_emission.Length() > SimplePT::EPSILON);
	m_has_specular = (m_ks.Length() > SimplePT::EPSILON);

	//m_map_kd = Vector3(0.0, 0.0, 0.0);
	m_texutre_name = material.diffuse_texname;
}

Vector3 Material::BRDF_PhongModel(const Vector3& wo, const Vector3& wi, const Vector3& normal) const
{
	Vector3 N = normal.Normalized();
	Vector3 unit_wi = wi.Normalized();
	Vector3 unit_wo = wo.Normalized();
	Vector3 reflected_unit_wi = 2 * DotProduct(unit_wi, N) * N - unit_wi;

	// diffuse
	Vector3 brdf_all;
	Vector3 all_kd = m_kd;
	if (m_HasMapKd())
	{
		all_kd = (m_kd.Length() < SimplePT::EPSILON) ? m_map_kd : all_kd * m_map_kd;
	}
	Vector3 brdf_diffuse = all_kd / SimplePT::PI;
	brdf_all += brdf_diffuse;

	// specular
	if (m_has_specular)
	{
		double cos_refwi_wo = DotProduct(reflected_unit_wi, wo); // cos(reflected_unit_wi, unit_wo);
		double cos_wi_N = DotProduct(unit_wi, N); // cos(reflected_unit_wi, unit_wo);
		Vector3 brdf_phong_specular = m_ks * std::pow(cos_refwi_wo, m_ns) / cos_wi_N;
		brdf_all += brdf_phong_specular;
	}

	return brdf_all;
}
