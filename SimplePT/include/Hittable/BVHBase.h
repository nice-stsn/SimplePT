#pragma once

#include "Hittable/Hittable.h"
#include "Vector.h"
#include <vector>
#include <cassert>

struct AABB
{
	AABB() 
		: m_min(Position3( SimplePT::INF,  SimplePT::INF,  SimplePT::INF)),
		  m_max(Position3(-SimplePT::INF, -SimplePT::INF, -SimplePT::INF)) 
	{}

	AABB& operator+=(Position3 point)
	{
		m_max.m_x = m_max.m_x > point.m_x ? m_max.m_x : point.m_x;
		m_max.m_y = m_max.m_y > point.m_y ? m_max.m_y : point.m_y;
		m_max.m_z = m_max.m_z > point.m_z ? m_max.m_z : point.m_z;

		m_min.m_x = m_min.m_x < point.m_x ? m_min.m_x : point.m_x;
		m_min.m_y = m_min.m_y < point.m_y ? m_min.m_y : point.m_y;
		m_min.m_z = m_min.m_z < point.m_z ? m_min.m_z : point.m_z;
		return *this;
	}

	AABB& operator+=(const AABB& other)
	{
		(*this) += other.m_max;
		(*this) += other.m_min;
		return *this;
	}

	bool Intersect(const Ray& ray, double& out_t_min, double& out_t_max) const
	{
		double t_min = 0.0, t_max = SimplePT::INF;
		for (int i = 0; i < 3; ++i)
		{
			double invD = 1.0 / ray.GetDirection()[i];
			double t0 = (m_min[i] - ray.GetOrigin()[i]) * invD;
			double t1 = (m_max[i] - ray.GetOrigin()[i]) * invD;
			if (invD < 0.0)
				std::swap(t0, t1);
			t_min = t0 > t_min ? t0 : t_min;
			t_max = t1 < t_max ? t1 : t_max;
			if (t_max + SimplePT::EPSILON < t_min)
			{
				out_t_min = -1.0;
				out_t_max = -1.0;
				return false;
			}
		}
		out_t_min = t_min;
		out_t_max = t_max;
		return true;
	}

	Position3 m_min, m_max;
};

struct BvhNodeBase
{
	unsigned int m_leftID, m_rightID;
	unsigned int m_start, m_len; // triangle index
	AABB m_box;
	bool m_IsLeaf() const { return m_len != 0; }
};

class BVHBase : public HittableBase
{
public:
	BVHBase() { }
	virtual ~BVHBase() = 0;

	void BuildBVH();
	virtual bool HitHappened(const Ray& ray, HitRecord& out_hit_record, double t_min = SimplePT::EPSILON, double t_max = SimplePT::INF) const override;

protected:
	std::vector<AABB> m_primitive_aabbs;		 // index by direct primitive index

private:
	std::vector<int> m_indirect_primitive_index; // direct primitive index stored 
	int indir2dir(int indirect) const { return m_indirect_primitive_index[indirect]; }
	std::vector<BvhNodeBase> m_nodes;			 // array of nodes
	unsigned int m_primitive_cnt = 0;
	unsigned int m_root_index = 0;
	unsigned int m_node_used = 0; 

	inline void m_SwapPrimitive(unsigned int i, unsigned int j)
	{
		assert(i < m_primitive_cnt && j < m_primitive_cnt);
		std::swap(m_indirect_primitive_index[i], m_indirect_primitive_index[j]);
	}
	void m_UpdateNodeBounds(unsigned int node_index);
	void m_SubdivideBuildBVH(unsigned int node_index);
	bool m_HitRecursive(unsigned int node_index, const Ray& ray, HitRecord& out_hit_record, double t_min, double t_max) const;

	//// virtual
	virtual bool m_Primitive_HitHappend(unsigned int node_index, const Ray& ray, HitRecord& out_hit_record, double t_min, double t_max) const = 0;
	virtual Vector3 m_GetPrimitiveCenter(unsigned int primitive_index) const = 0;
	virtual unsigned int m_GetPrimitiveNumber() const = 0;
	virtual void m_InitPrimitiveAABBs() = 0;
};
