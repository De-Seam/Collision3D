#pragma once
#include "utils/math.h"
#include "shapes.h"

#include <vector>

struct BVHNode
{
	fm::AABB aabb;
	int32_t left_first = 0;
	int32_t count = 0;
};

class BVH
{
public:
	virtual size_t add_shape(Shape* shape);

	void draw();

	void generate();

	void broadphase_collisions(const fm::AABB& aabb, size_t id);

	std::vector<std::vector<Shape*>>& get_manifest() { return m_collision_manifest; } 
protected:
	void collisions_internal(const fm::AABB& aabb, size_t id, size_t node);

	unsigned int* m_indices = nullptr;
	std::vector<BVHNode> m_nodes = {};
	std::vector<Shape*> m_shapes = {};

	std::vector<std::vector<Shape*>> m_collision_manifest = {};

	virtual void subdivide(BVHNode* node, int first, int count, int depth);
	virtual void partition(BVHNode* node, int first, int count, int depth);

	fm::AABB create_aabb_from_shapes(size_t begin, size_t end);
	float compute_split_cost(BVHNode* node, int first, int count, float split_location, int split_axis);
	int split_indices(BVHNode* node, int first, int count, float split_location, int split_axis);

private:
};