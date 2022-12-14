#include "collision/shapes.h"

#include "app.h"

bool SphereShape::hits_ray(fm::Ray ray, float& max_distance)
{
	fm::vec3 m = ray.origin - m_position;
	float b = m.dot(ray.direction);
	float c = m.dot(m) - (m_radius * m_radius);

	if(c > 0.f && b > 0.f)
		return false;

	float discr = b * b - c;
	if(discr < 0.f)
		return false;

	float distance = -b - std::sqrtf(discr);

	if(distance < 0.f)
		distance = 0.f;

	if(distance < max_distance)
	{
		max_distance = distance;
		return true;
	}
}

bool AABBShape::hits_ray(fm::Ray ray, float& max_distance)
{
	fm::AABB aabb = get_aabb();

	float tmin = (aabb.min.x - ray.origin.x) / ray.direction.x; 
	float tmax = (aabb.max.x - ray.origin.x) / ray.direction.x; 
 
	if (tmin > tmax) std::swap(tmin, tmax); 
 
	float tymin = (aabb.min.y - ray.origin.y) / ray.direction.y; 
	float tymax = (aabb.max.y - ray.origin.y) / ray.direction.y; 
 
	if (tymin > tymax) std::swap(tymin, tymax); 
 
	if ((tmin > tymax) || (tymin > tmax)) 
		return false; 
 
	if (tymin > tmin) 
		tmin = tymin; 
 
	if (tymax < tmax) 
		tmax = tymax; 
 
	float tzmin = (aabb.min.z - ray.origin.z) / ray.direction.z; 
	float tzmax = (aabb.max.z - ray.origin.z) / ray.direction.z; 
 
	if (tzmin > tzmax) std::swap(tzmin, tzmax); 
 
	if ((tmin > tzmax) || (tzmin > tmax)) 
		return false; 
 
	if (tzmin > tmin) 
		tmin = tzmin; 
 
	if (tzmax < tmax) 
		tmax = tzmax; 

	if(tmin < max_distance)
	{
		max_distance = tmin;
		return true; 
	}
	return false;
}

bool ModelShape::hits_ray(fm::Ray ray, float& max_distance)
{
	Ray rl_ray;
	rl_ray.position = {ray.origin.x, ray.origin.y, ray.origin.z};
	rl_ray.direction = {ray.direction.x, ray.direction.y, ray.direction.z};

	//memcpy(&rl_transform, &model_matrix, sizeof(fm::mat4));

	RayCollision ray_c = GetRayCollisionMesh(rl_ray, m_model.meshes[0], m_model.transform);
	if(ray_c.hit && ray_c.distance < max_distance)
	{
		max_distance = ray_c.distance;
		return true;
	}

	return false;
}