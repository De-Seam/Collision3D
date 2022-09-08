#include "collision/functions.h"

bool collides(const fm::AABB& a, const fm::AABB& b)
{
	return 
		a.min.x < b.max.x &&
		a.max.x > b.min.x &&
		a.min.y < b.max.y &&
		a.max.y > b.min.y &&
		a.min.z < b.max.z &&
		a.max.z > b.min.z;
}

void detect_collision(AABBShape* a, AABBShape* b)
{
	if(collides(a->get_aabb(), b->get_aabb()))
	{
		a->add_collision();
		b->add_collision();
	}
}