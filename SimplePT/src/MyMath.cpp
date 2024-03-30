#include "MyMath.h"
#include <limits>

namespace SimplePT 
{
	const double PI = 3.141592653589793;
	const double EPSILON = 0.0000001;
	const double INF = std::numeric_limits<double>::max();

	static Vector3 ToWorld(const Vector3& normal, const Vector3& vec)
	{
	   // Using right-hand coord
		Vector3 up = std::abs(normal.m_y) < 0.999 ? Vector3(0.0, 1.0, 0.0) : Vector3(1.0, 0.0, 0.0);
		Vector3 xAxis = CrossProduct(up, normal);
		Vector3 yAxis = CrossProduct(normal, xAxis);
		Vector3 zAxis = normal;

		Vector3 world_vec = vec.m_x * xAxis + vec.m_y * yAxis + vec.m_z * zAxis;
		return world_vec;
	}

	void Sample_Hemisphere_Uniform(const Vector3& input_normal, Vector3& output_wi, double& output_pdf_of_wi)
	{
		double u1 = GetRandomDouble_0_to_1();
		double u2 = GetRandomDouble_0_to_1();
		double phi = 2 * PI * u2;
		double z = u1;
		double sin_theta = 1 - z * z;
		double x = sin_theta * std::cos(phi);
		double y = sin_theta * std::sin(phi);

		Vector3 wi(x, y, z);
		wi = ToWorld(input_normal, wi);

		output_wi = wi;
		output_pdf_of_wi = 1 / (2 * PI);
	}

		
	void Sample_Hemisphere_Cos_Weighted(const Vector3& input_normal, Vector3& output_wi, double& output_pdf_of_wi)
	{
		double u1 = GetRandomDouble_0_to_1();
		double u2 = GetRandomDouble_0_to_1();
		double phi = 2 * PI * u2;
		double theta = 0.5 * std::acos(1 - 2 * u1);
		double z = std::cos(theta);
		double sin_theta = 1 - z * z;
		double x = sin_theta * std::cos(phi);
		double y = sin_theta * std::sin(phi);

		Vector3 wi(x, y, z);
		wi = ToWorld(input_normal, wi);

		output_wi = wi;
		output_pdf_of_wi = z / PI;
	}

}
