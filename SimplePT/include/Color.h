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
	Color3() : m_R(0.0f), m_G(0.0f), m_B(0.0f) {}
	explicit Color3(unsigned char R, unsigned char G, unsigned B)
		: m_R(R), m_G(G), m_B(B) {}

	explicit Color3(double dR, double dG, double dB)
		: m_R(static_cast<int>(clamp_to_zero_one(dR) * 255.999))
		, m_G(static_cast<int>(clamp_to_zero_one(dG) * 255.999))
		, m_B(static_cast<int>(clamp_to_zero_one(dB) * 255.999))
	{}

	inline unsigned char R() const { return m_R; }
	inline unsigned char G() const { return m_G; }
	inline unsigned char B() const { return m_B; }

private:
	unsigned char m_R, m_G, m_B;
};
