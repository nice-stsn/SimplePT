#pragma once
#include "Vector.h"
#include "Color.h"
#include "tiny_obj_loader.h"

class Material
{
public:
	Material() = default;
	Material(const tinyobj::material_t& material);
	Material(const Vector3& set_emission) : m_emission(set_emission), m_kd(0.0, 0.0, 0.0) {}
	~Material() {}
	inline bool HasEmission() const { return m_has_emission;  }
	inline Vector3 GetEmission() const { return m_emission; }

private:
	bool m_has_emission = false;
	Vector3 m_emission;
	Vector3 m_kd;
};
