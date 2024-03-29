#pragma once
#include "Vector.h"
#include "Color.h"
#include "tiny_obj_loader.h"

class Material
{
public:
	Material() = default;
	Material(const tinyobj::material_t& material);
	//Material(const Vector3& set_emission) 
	//	: m_emission(set_emission), m_kd(0.0, 0.0, 0.0) 
	//{}
	~Material() {}
	inline bool HasEmission() const { return m_has_emission;  }
	inline bool HasSpecular() const { return m_has_specular;  }
	inline Vector3 GetEmission() const { return m_emission; }
	inline Vector3 GetKd() const { return m_kd; }
	Vector3 BRDF_PhongModel(const Vector3& wo, const Vector3& wi, const Vector3& normal) const;

private:
	bool m_has_emission = false;
	bool m_has_specular = false;
	Vector3 m_emission;
	Vector3 m_kd;
	Vector3 m_ks;
	Vector3 m_tr;
	double  m_ns = 0;
	double  m_ior = 0;

};
