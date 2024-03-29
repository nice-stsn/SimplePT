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
		return std::abs(a - b) < 0.01;
	}

	inline bool Equal(double a, double b)
	{
		return std::abs(a - b) < EPSILON;
	}

	inline void GammaCorrection(Vector3& radiance, double gamma = 2.2)
	{
		double inv_gamma = 1 / gamma;
		radiance.m_x = std::pow(radiance.m_x, inv_gamma);
		radiance.m_y = std::pow(radiance.m_y, inv_gamma);
		radiance.m_z = std::pow(radiance.m_z, inv_gamma);
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
