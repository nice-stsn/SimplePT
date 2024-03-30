#pragma once
#include "Vector.h"
#include "Color.h"
#include "MyMath.h"
#include "tiny_obj_loader.h"

class Material
{
public:
	Material() = default;
	Material(const tinyobj::material_t& material);
	~Material() {}
	inline bool isMirror() const { return m_ns >= 9999;  } // for bathroom scene
	inline bool HasEmission() const { return m_has_emission;  }
	inline bool HasSpecular() const { return m_has_specular;  }
	inline Vector3 GetEmission() const { return m_emission; }
	inline double GetNs() const { return m_ns; }
	inline Vector3 GetKd() const { return m_kd; }
	inline std::string GetTexutreName() const { return m_texutre_name; }

	inline void SetMapKd(const Vector3& map_kd) { m_map_kd = map_kd; }
	Vector3 BRDF_PhongModel(const Vector3& wo, const Vector3& wi, const Vector3& normal) const;

private:
	inline bool m_HasMapKd() const { return m_map_kd.Length() > SimplePT::EPSILON;  }

private:
	bool m_has_emission = false;
	bool m_has_specular = false;
	Vector3 m_emission;
	Vector3 m_kd;
	Vector3 m_ks;
	Vector3 m_tr;
	std::string  m_texutre_name;
	Vector3 m_map_kd;
	double  m_ns = 0;
	double  m_ior = 0;

};
