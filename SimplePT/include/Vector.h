#pragma once


class Vector3
{
public:
	Vector3() = default;
	Vector3(double x, double y, double z) : m_x(x), m_y(y), m_z(z) {}
	~Vector3() {}

private:
	double m_x, m_y, m_z;
};


using Position3 = Vector3;
