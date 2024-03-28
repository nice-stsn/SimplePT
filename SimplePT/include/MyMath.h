#pragma once

#include <cmath>
#include <random>
#include "Vector.h"

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
		return std::abs(a - b) < 0.0001;
	}

	inline bool Equal(double a, double b)
	{
		return std::abs(a - b) < EPSILON;
	}


	// [0, 1) uniform
	inline double GetRandomDouble_0_to_1()
	{
		//static std::uniform_real_distribution<double> distribution(0.0, 1.0);
		//static std::mt19937 generator;
		//return distribution(generator);
		return rand() / (RAND_MAX + 1.0);
	}

	// [min, max) uniform
	inline double GetRandomDouble_min_to_max(double min, double max) {
		return min + (max - min) * GetRandomDouble_0_to_1();
	}

	void Sample_Hemisphere_Uniform(const Vector3& input_normal, Vector3& output_wi, double& output_pdf_of_wi);
	void Sample_Hemisphere_Cos_Weighted(const Vector3& input_normal, Vector3& output_wi, double& output_pdf_of_wi);

}
