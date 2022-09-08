#pragma once
#include "utils/math.h"

namespace fm
{
	struct Ray
	{
		fm::vec3 origin;
		fm::vec3 direction; //Normal
	};

	struct AABB
	{
		fm::vec3 min, max;
	};
}

enum class ShapeType
{
	Invalid,
	Sphere,
	AABB,
	Cube
};

struct Shape
{
	virtual void draw() = 0;
	virtual bool hits_ray(fm::Ray ray, float& max_distance) = 0;

	void add_collision() {m_collision_count++;} //Call for every collision
	void reset_collisions() {m_collision_count = 0;} //Called by app
	ShapeType get_type() { return m_type; }

	fm::vec3 m_position;
	int m_collision_count = 0;
	ShapeType m_type = ShapeType::Invalid;
};

struct SphereShape : public Shape
{
	SphereShape(fm::vec3 pos, float r) : m_radius(r) {m_position = pos; m_type = ShapeType::Sphere;}
	virtual void draw() override;
	virtual bool hits_ray(fm::Ray ray, float& max_distance) override;

	float m_radius;
};

struct AABBShape : public Shape
{
	AABBShape(fm::vec3 pos, fm::vec3 size)
	{
		m_position = pos;
		m_size = size;
		m_type = ShapeType::AABB;
	}

	virtual void draw() override;
	virtual bool hits_ray(fm::Ray ray, float& max_distance) override;

	fm::AABB get_aabb()
	{
		fm::vec3 half_size = m_size * 0.5f;
		return {m_position - half_size, m_position + half_size};
	}

	fm::vec3 m_size;

};