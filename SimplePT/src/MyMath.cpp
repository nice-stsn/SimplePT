#include "MyMath.h"
#include <limits>

namespace SimplePT 
{
	const double PI = 3.141592653589793;
	const double EPSILON = 0.0000001;
	const double INF = std::numeric_limits<double>::max();


	void Sample_Hemisphere_Uniform(const Vector3& input_normal, Vector3& output_wi, double& output_pdf_of_wi)
	{
		// rejection method // replace later
		double rand_x = 1.0, rand_y = 1.0, rand_z = 1.0;
		while (rand_x * rand_x + rand_y * rand_y + rand_z * rand_z > 1.0)
		{
			// random on sphere, not hemisphere
			rand_x = SimplePT::GetRandomDouble_min_to_max(-1, 1);
			rand_y = SimplePT::GetRandomDouble_min_to_max(-1, 1);
			rand_z = SimplePT::GetRandomDouble_min_to_max(-1, 1);
		}

		Vector3 wi(rand_x, rand_y, rand_z);
		wi = wi.Normalized();

		// hemisphere
		if (DotProduct(wi, input_normal) < 0)
		{
			wi = (-wi);
		}
		output_wi = wi;
		output_pdf_of_wi = 1 / (2 * PI);
	}
}
