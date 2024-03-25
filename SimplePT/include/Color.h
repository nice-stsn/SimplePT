#pragma once

#include "Vector.h"


inline double clamp_to_zero_one(double d)
{
	if (d > 1.0)
		return 1.0f;
	else if (d < 0.0)
		return 0.0f;
	else
		return d;
}

class Color3
{
public:
	Color3() : m_R(0), m_G(0), m_B(0) {}
	explicit Color3(unsigned char R, unsigned char G, unsigned B)
		: m_R(R), m_G(G), m_B(B) {}

	explicit Color3(double dR, double dG, double dB)
		: m_R(static_cast<int>(clamp_to_zero_one(dR) * 255.999))
		, m_G(static_cast<int>(clamp_to_zero_one(dG) * 255.999))
		, m_B(static_cast<int>(clamp_to_zero_one(dB) * 255.999))
	{}

	explicit Color3(Vector3 d_color)
		: Color3(d_color.m_x, d_color.m_y, d_color.m_z)
	{}

	inline unsigned char R() const { return m_R; }
	inline unsigned char G() const { return m_G; }
	inline unsigned char B() const { return m_B; }

	inline Color3& operator+=(const Color3& rhs)
	{
		int r = m_R + rhs.m_R;
		int g = m_G + rhs.m_G;
		int b = m_B + rhs.m_B;
		m_R = m_clamp_color(r);
		m_G = m_clamp_color(g);
		m_B = m_clamp_color(b);
		return *this;
	}


private:
	inline unsigned char m_clamp_color(int color) {
		if (color > 255)
			return static_cast<unsigned char>(255);
		else if (color < 0)
			return static_cast<unsigned char>(0);
		else
			return static_cast<unsigned char>(color);
	}
	unsigned char m_R, m_G, m_B;
};
