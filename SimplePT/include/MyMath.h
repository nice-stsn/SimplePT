#pragma once

#include <cmath>

namespace SimplePT {
	extern const double PI;
	extern const double EPSILON;
	extern const double INF;

	inline double deg2rad(double deg)
	{
		return deg / 180 * PI;
	}
	
	inline bool Equal(double a, double b)
	{
		return std::abs(a - b) < EPSILON;
	}



}
