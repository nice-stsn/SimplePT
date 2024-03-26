#include "Hittable/BVHBase.h"
#include <cassert>
#include <numeric>


BVHBase::~BVHBase() {}


void BVHBase::BuildBVH()
{
	// ref: https://jacco.ompf2.com/2022/04/13/how-to-build-a-bvh-part-1-basics/
	//init members
	m_primitive_cnt = m_GetPrimitiveNumber();
	m_primitive_aabbs.resize(m_primitive_cnt);
	m_indirect_primitive_index.resize(m_primitive_cnt);
	m_nodes.resize(2 * m_primitive_cnt);
	// todo: iota
	std::iota(m_indirect_primitive_index.begin(), m_indirect_primitive_index.end(), 0);
	//for (unsigned int i = 0; i < m_primitive_cnt; ++i)
	//{
	//	m_indirect_primitive_index[i] = i;
	//}
	m_InitPrimitiveAABBs();

	// assign all primitive to root node
	BvhNodeBase& root = m_nodes[m_root_index];
	root.m_leftID = 0;
	root.m_rightID = 0;
	root.m_start = 0;
	root.m_len = m_primitive_cnt;
	m_node_used++;

	// subdivide recursively
	m_SubdivideBuildBVH(m_root_index);

}

bool BVHBase::HitHappened(const Ray& ray, HitRecord& out_hit_record, double t_min, double t_max) const
{
	//// traverse BVH tree
	return m_HitRecursive(m_root_index, ray, out_hit_record, t_min, t_max);
}

void BVHBase::m_SubdivideBuildBVH(unsigned int node_index)
{
	m_UpdateNodeBounds(node_index);

	// Determine the axis and position of the split plane
	BvhNodeBase& node = m_nodes[node_index];
	if (node.m_len <= 1) 
		return;

	Vector3 extent = node.m_box.m_max - node.m_box.m_min;

	// Choosing axis
	int axis = 0;
	if (extent[1] > extent[axis]) axis = 1;
	if (extent[2] > extent[axis]) axis = 2;
	
	// try x y z axis, when one failed
	double split_pos = node.m_box.m_min[axis] + extent[axis] / 2;
	int i = node.m_start;
	int j = node.m_start + node.m_len - 1;
	while (i <= j)
	{
		Vector3 center = m_GetPrimitiveCenter(indir2dir(i));
		if (center[axis] < split_pos) {
			++i;
		} else {
			m_SwapPrimitive(i, j);
			--j;
		}
	}

	// Create child nodes for each half
	unsigned int left_cnt = i - node.m_start;
	unsigned int right_cnt = node.m_len - left_cnt;

	// if find empty split 
	if (left_cnt == 0 || right_cnt == 0) {
		if (node.m_len >= 2) {
			left_cnt = node.m_len / 2;
			right_cnt = node.m_len - left_cnt;
		}
		else {
			// todo: node.m_len > 2 still
			assert(node.m_len <= 1);
			return;
		}
	}
	unsigned int leftID  = m_node_used++;
	unsigned int rightID = m_node_used++;
	node.m_leftID = leftID;
	node.m_rightID = rightID;
	m_nodes[leftID].m_start = node.m_start;
	m_nodes[leftID].m_len = left_cnt;
	m_nodes[rightID].m_start = node.m_start + left_cnt;
	m_nodes[rightID].m_len = right_cnt;

	node.m_len = 0; // this node is not leaf now

	// Recursive call
	m_SubdivideBuildBVH(leftID);
	m_SubdivideBuildBVH(rightID);

}

bool BVHBase::m_HitRecursive(unsigned int node_index, const Ray& ray, HitRecord& out_hit_record, double t_min, double t_max) const
{
	const BvhNodeBase& node = m_nodes[node_index];
	double this_box_t_min = -1.0, this_box_t_max = -1.0;
	if (!node.m_box.Intersect(ray, this_box_t_min, this_box_t_max))
	{
		return false;
	}

	// if intersect with leaf node
	if (node.m_IsLeaf()) {
		// check  primitive
		return m_Primitive_HitHappend(indir2dir(node.m_start), ray, out_hit_record, t_min, t_max);
	}

	HitRecord l_rec, r_rec;
	bool l_hit = m_HitRecursive(node.m_leftID, ray, l_rec, t_min, t_max);
	bool r_hit = m_HitRecursive(node.m_rightID, ray, r_rec, t_min, t_max);

	if (!l_hit && !r_hit)
		return false;
	else if (l_hit && !r_hit)
		out_hit_record = l_rec;
	else if (!l_hit && r_hit)
		out_hit_record = r_rec;
	else if (l_hit && r_hit)
	{
		out_hit_record = l_rec.m_t < r_rec.m_t ? l_rec : r_rec;
	}

	return true;
}


void BVHBase::m_UpdateNodeBounds(unsigned int node_index)
{
	// Determine the axis and position of the split plane
	BvhNodeBase& node = m_nodes[node_index];

	unsigned int start = node.m_start;
	unsigned int len = node.m_len;

	for (unsigned int i = 0; i < len; ++i)
	{
		unsigned int primitive_index = m_indirect_primitive_index[start + i];
		node.m_box += m_primitive_aabbs[primitive_index];
	}

}
