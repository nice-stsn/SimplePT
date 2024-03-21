#pragma once

#include <stdexcept>
#include <cmath>

class Vector3
{
public:
	Vector3() = default;
	Vector3(double x, double y, double z) : m_x(x), m_y(y), m_z(z) {}
	~Vector3() {}

	// Unary operatro
	inline Vector3 operator-() const
	{
		return Vector3(-m_x, -m_y, -m_z);
	}

	inline Vector3& operator+=(const Vector3& rhs)
	{
		m_x += rhs.m_x;
		m_y += rhs.m_y;
		m_z += rhs.m_z;
		return *this;
	}
	inline Vector3& operator-=(const Vector3& rhs)
	{
		m_x -= rhs.m_x;
		m_y -= rhs.m_y;
		m_z -= rhs.m_z;
		return *this;
	}
	inline Vector3& operator*=(double val)
	{
		m_x *= val;
		m_y *= val;
		m_z *= val;
		return *this;
	}
	inline Vector3& operator/=(double val)
	{
		if (val == 0.0f)
			throw std::invalid_argument("Cannot divide by zero.");

		double inv_val = 1.0 / val;
		*this *= inv_val;
		return *this;
	}

	// Binary operator
	inline Vector3  operator+(const Vector3& rhs)const
	{
		return Vector3(m_x + rhs.m_x, m_y + rhs.m_y, m_z + rhs.m_z);
	}

	inline Vector3  operator-(const Vector3& rhs)const
	{
		return Vector3(m_x - rhs.m_x, m_y - rhs.m_y, m_z - rhs.m_z);
	}

	inline Vector3  operator*(double val)const
	{
		return Vector3(m_x * val, m_y * val, m_z * val);
	}

	inline Vector3  operator/(double val)const
	{
		if (val == 0.0f)
			throw std::invalid_argument("Cannot divide by zero.");

		double inv_val = 1.0 / val;
		return (*this) * inv_val;
	}

	inline Vector3 Normalized() const
	{
		double len = std::sqrt(m_x * m_x + m_y * m_y + m_z * m_z);
		return Vector3(m_x / len, m_y / len, m_z / len);
	}

	double m_x, m_y, m_z;
};

inline Vector3 Cross(const Vector3& vl, const Vector3& vr)
{
	return Vector3(
		vl.m_y * vr.m_z - vl.m_z * vr.m_y,
		vl.m_z * vr.m_x - vl.m_x * vr.m_z,
		vl.m_x * vr.m_y - vl.m_y * vr.m_x );
}

inline Vector3 operator*(double t, const Vector3& vec)
{
	return Vector3(t * vec.m_x, t * vec.m_y, t * vec.m_z);
}


using Position3 = Vector3;
