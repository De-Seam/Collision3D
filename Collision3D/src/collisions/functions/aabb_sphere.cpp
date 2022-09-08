#include "collision/functions.h"

void detect_collision(AABBShape* a, SphereShape* b)
{
	fm::AABB aabb = a->get_aabb();
	float dmin = 0.f;
	for(size_t i = 0; i < 3; i++ ) 
	{
		float v = b->m_position[i];
		if( v < aabb.min[i] ) dmin += (aabb.min[i] - v) * (aabb.min[i] - v);
		if( v > aabb.max[i] ) dmin += (v - aabb.max[i]) * (v - aabb.max[i]);   
	}
	float radius2 = b->m_radius * b->m_radius;
	if( dmin <= radius2 )
	{
		a->add_collision();
		b->add_collision();
	}
}