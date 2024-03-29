#include "Hittable/Hittable.h"

HittableBase::~HittableBase() 
{
}

void HittableBase::ExtractLightInfo(unsigned int actor_id, SceneLightInfo& out_info) const
{
}

void HittableBase::SampleLight_ByPrimitiveID(unsigned int primitive_id, HitRecord& out_sample_info, double& local_pdf) const
{
}