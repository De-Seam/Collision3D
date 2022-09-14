#pragma once
#include "utils/math.h"

#include <raylib/raylib.h>

#include <array>

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

		float area()
		{
			fm::vec3 length = max - min;
			return (2.0f * (length.x * length.y + length.y * length.z + length.z * length.x)); 
		}
	};
}

enum class ShapeType
{
	Invalid,
	Sphere,
	AABB,
	Model
};

struct Shape
{
	virtual void draw() = 0;
	virtual bool hits_ray(fm::Ray ray, float& max_distance) = 0;

	void add_collision() {m_collision_count++;} //Call for every collision
	void reset_collisions() {m_collision_count = 0;} //Called by app
	ShapeType get_type() { return m_type; }

	virtual void calculate_aabb() = 0;
	virtual fm::vec3 find_furthest_point(fm::vec3 direction) = 0;

	fm::vec3 m_position;
	int m_collision_count = 0;
	ShapeType m_type = ShapeType::Invalid;

	fm::AABB m_aabb;
};

struct SphereShape : public Shape
{
	SphereShape(fm::vec3 pos, float r) : m_radius(r) {m_position = pos; m_type = ShapeType::Sphere;}
	virtual void draw() override;
	virtual bool hits_ray(fm::Ray ray, float& max_distance) override;

	virtual void calculate_aabb() override
	{
		m_aabb = {m_position - fm::vec3{m_radius}, m_position + fm::vec3{m_radius}};
	}

	virtual fm::vec3 find_furthest_point(fm::vec3 direction) override
	{
		return m_position + fm::normalize_safe(direction) * m_radius;
	}

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

	virtual void calculate_aabb() override
	{
		fm::vec3 half_size = m_size * 0.5f;
		m_aabb = {m_position - half_size, m_position + half_size};
	}

	const fm::AABB& get_aabb()
	{
		return m_aabb;
	}

	virtual fm::vec3 find_furthest_point(fm::vec3 direction) override
	{
		fm::vec3 max_point;
		float max_distance = -INFINITY;

		fm::AABB aabb = m_aabb;
		fm::vec3 size = aabb.max - aabb.min;

		std::array<fm::vec3, 8> vertices
		{
			aabb.min, //0,0,0
			aabb.min + fm::vec3{size.x, 0, 0}, //1,0,0
			aabb.min + fm::vec3{size.x, 0, size.z}, //1,0,1
			aabb.min + fm::vec3{0, 0, size.z}, //0,0,1
			aabb.min + fm::vec3{0, size.y, size.z}, //0,1,1
			aabb.max,//1,1,1
			aabb.min + fm::vec3{size.x, size.y, 0}, //1,1,0
			aabb.min + fm::vec3{0, size.y, 0}, //0,1,1
		};

		for(int i = 0; i < 8; i++)
		{
			fm::vec3 vertex = vertices[i];
			float distance = vertex.dot(direction);
			if(distance > max_distance)
			{
				max_distance = distance;
				max_point = vertex;
			}
		}

		return max_point;
	}

	fm::vec3 m_size;

};

struct ModelShape : public Shape
{
	ModelShape(const std::string& file, fm::vec3 position, fm::vec3 scale, fm::vec3 rotation)
	{
		m_type = ShapeType::Model;
		m_model = LoadModel(file.c_str());
		m_position = position;
		m_scale = scale;
		m_rotation = rotation;
	}

	virtual void draw() override;
	virtual bool hits_ray(fm::Ray ray, float& max_distance) override;

	virtual void calculate_aabb() override
	{
		fm::vec3 min = {INFINITY};
		fm::vec3 max = {-INFINITY};
		for(int i = 0; i < m_model.meshes[0].vertexCount; i++)
		{
			min[i%3] = fm::min(m_model.meshes[0].vertices[i], min[i%3]);
			max[i%3] = fm::max(m_model.meshes[0].vertices[i], max[i%3]);
		}

		min *= m_scale;
		max *= m_scale;

		min+=m_position;
		max+=m_position;

		m_aabb = {min,max};
	}

	virtual fm::vec3 find_furthest_point(fm::vec3 direction) override
	{
		int vert_count = m_model.meshes[0].vertexCount;
		fm::vec3 max_point;
		float max_distance = -INFINITY;

		for(int i = 0; i < vert_count; i+=3)
		{
			fm::vec3 vertex = {m_model.meshes[0].vertices[i], m_model.meshes[0].vertices[i+1], m_model.meshes[0].vertices[i+2]};
			vertex *= m_scale;
			vertex += m_position;
			float distance = vertex.dot(direction);
			if(distance > max_distance)
			{
				max_distance = distance;
				max_point = vertex;
			}
		}

		return max_point;
	}

	Model m_model;
	fm::vec3 m_scale;
	fm::vec3 m_rotation;
};