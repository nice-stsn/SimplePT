#pragma once

#include <cmath>

namespace SimplePT {
	extern const double PI;

	inline double deg2rad(double deg)
	{
		return deg / 180 * PI;
	}

}
