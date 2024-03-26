#pragma once

#include <cmath>
#include <random>

namespace SimplePT {
	extern const double PI;
	extern const double EPSILON;
	extern const double INF;

	inline double deg2rad(double deg)
	{
		return deg / 180 * PI;
	}
	
	inline bool EqualApprox(double a, double b)
	{
		return std::abs(a - b) < 0.01;
	}

	inline bool Equal(double a, double b)
	{
		return std::abs(a - b) < EPSILON;
	}

	inline double GetRandomDouble_0_to_1()
	{
		static std::default_random_engine generator;
		std::uniform_real_distribution<double> distribution(0.0, 1.0);

		return distribution(generator);
	}




}
