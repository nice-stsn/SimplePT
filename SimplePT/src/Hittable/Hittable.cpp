#include "Hittable/Hittable.h"

void HitRecord::SampleSpecular (const Vector3& input_unit_wo, Vector3& out_unit_wi,
		double& out_pdf_of_wi, bool& out_wi_under_hit_surface) const
{
	// find wi & pdf_of_wi
	const Vector3& Normal = m_hit_unit_normal;
	const Vector3& R = SimplePT::Reflect(Normal, input_unit_wo);
	
	// specular is arount R
	//SimplePT::Sample_Hemisphere_Cos_Weighted(R, out_unit_wi, out_pdf_of_wi);
	//SimplePT::Sample_Hemisphere_Cos_Ns_Weighted(m_material.GetNs(), R, out_unit_wi, out_pdf_of_wi);
	SimplePT::Sample_Hemisphere_Cos_Weighted(R, out_unit_wi, out_pdf_of_wi);

	// check under
	out_wi_under_hit_surface = DotProduct(out_unit_wi, Normal) < 0;
}

HittableBase::~HittableBase() 
{
}
