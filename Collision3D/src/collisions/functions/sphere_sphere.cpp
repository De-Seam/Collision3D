#include "collision/functions.h"

void detect_collision(SphereShape* a, SphereShape* b)
{
	fm::vec3 p0 = a->m_position;
	fm::vec3 p1 = b->m_position;
	float distance2 = 
		((p1.x - p0.x) * (p1.x - p0.x) +
		(p1.y - p0.y) * (p1.y - p0.y) +
		(p1.z - p0.z) * (p1.z - p0.z));
	float max_distance = a->m_radius + b->m_radius;
	float max_distance2 = max_distance * max_distance;

	if(distance2 <= max_distance2)
	{
		a->add_collision();
		b->add_collision();
	}
}