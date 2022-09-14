#include "collision/bvh.h"

#include <raylib/raylib.h>


size_t BVH::add_shape(Shape* shape)
{
	m_shapes.emplace_back(shape);
	return m_shapes.size()-1;
}

void BVH::draw()
{
	for(size_t i = 0; i < m_nodes.size(); i++)
	{
		fm::vec3 size = m_nodes[i].aabb.max - m_nodes[i].aabb.min;
		fm::vec3 half_size = size * 0.5f;
		fm::vec3 position = m_nodes[i].aabb.min + half_size;
		DrawCubeWires({position.x,position.y,position.z}, size.x, size.y, size.z, GRAY);
	}

	for(size_t i = 0; i < m_shapes.size(); i++)
	{
		fm::vec3 size = m_shapes[i]->m_aabb.max - m_shapes[i]->m_aabb.min;
		fm::vec3 half_size = size * 0.5f;
		fm::vec3 position = m_shapes[i]->m_aabb.min + half_size;
		DrawCubeWires({position.x,position.y,position.z}, size.x, size.y, size.z, GRAY);
	}
}

void BVH::generate()
{
	if(m_indices)
		_aligned_free(m_indices);

	m_nodes.clear();

	m_indices = static_cast<unsigned int*>(_aligned_malloc((static_cast<size_t>(m_shapes.size())+1)*4, 64));
	for(size_t i = 0; i < m_shapes.size(); i++)
		m_indices[i] = static_cast<unsigned int>(i);

	m_nodes.reserve(m_shapes.size() * 2 + 1);
	m_nodes.emplace_back(BVHNode()); //Root Node
	m_nodes.emplace_back(BVHNode()); //Empty Node for cache alignment

	m_nodes[0].aabb = create_aabb_from_shapes(0, m_shapes.size());
	subdivide(&m_nodes[0], 0, m_shapes.size(), 0);

	printf("BVH Created with %i nodes\n", m_nodes.size());
}

void BVH::subdivide(BVHNode* node, int first, int count, int depth)
{
	node->aabb = create_aabb_from_shapes(first, first + count);
	partition(node, first, count, depth);
}

void BVH::partition(BVHNode* node, int first, int count, int depth)
{
	fm::vec3 sides_length = node->aabb.max - node->aabb.min;
	float best_split_location = 0;
	float best_split_cost = INFINITY;
	size_t best_split_axis = 0;

	int split_amount = 20;
	if(count < split_amount)
		split_amount = count;
	float split_amount_flt = static_cast<float>(split_amount);
	float split_amount_inv_flt = 1.f / split_amount_flt;

	for(size_t axis = 0; axis < 3; axis++)
	{
		float next_val = 0;
		for(size_t i = 0; i < split_amount; i++)
		{
			next_val += sides_length[axis] * split_amount_inv_flt;
			float split_cost = compute_split_cost(node, first, count, node->aabb.min[axis] + next_val, axis);
			if(split_cost < best_split_cost)
			{
				best_split_cost = split_cost;
				best_split_location = node->aabb.min[axis] + next_val;
				best_split_axis = axis;
			}
		}
	}

	//Leaf node
	if(best_split_cost >= node->aabb.area() * static_cast<float>(count))
	{
		node->left_first = first;
		node->count = count;
		return;
	}

	int index = split_indices(node, first, count, best_split_location, best_split_axis);
	int left_count = index - first;

	node->left_first = m_nodes.size();

	//Children
	m_nodes.emplace_back(BVHNode());
	m_nodes.emplace_back(BVHNode());

	subdivide(&m_nodes[node->left_first], first, left_count, depth+1);
	subdivide(&m_nodes[node->left_first+1], index, count-left_count, depth+1);
}

fm::AABB BVH::create_aabb_from_shapes(size_t begin, size_t end)
{
	//This has to be infinity. Don't try to optimize it, edge cases will fail and asserts will be thrown.
	fm::vec3 min = {INFINITY};
	fm::vec3 max = {-INFINITY};

	for(size_t i = begin; i < end; i++)
	{
		for(size_t axis = 0; axis < 3; axis++)
		{
			min[axis] = fm::min(min[axis], m_shapes[i]->m_aabb.min[axis]);
			max[axis] = fm::max(max[axis], m_shapes[i]->m_aabb.max[axis]);
		}
	}

	return {min,max};
}

float BVH::compute_split_cost(BVHNode* node, int first, int count, float split_location, int split_axis)
{
	int index = split_indices(node, first, count, split_location, split_axis);

	int count_left = index - first;
	int count_right = count - count_left;

	fm::AABB aabb_left = create_aabb_from_shapes(first, first + count_left);
	fm::AABB aabb_right = create_aabb_from_shapes(index, index + count_right);

	return aabb_left.area() * count_left + aabb_right.area() + count_right;
}

int BVH::split_indices(BVHNode* node, int first, int count, float split_location, int split_axis)
{
	int index = first;
	for(int i = first; i < first + count; i++)
	{
		if(m_shapes[m_indices[i]]->m_position[split_axis] <= split_location)
		{
			//std::swap(m_indices[index], m_indices[i]);
			unsigned int t = m_indices[index];
			m_indices[index] = m_indices[i];
			m_indices[i] = t;
			index++;
		}
	}
	return index;
}
