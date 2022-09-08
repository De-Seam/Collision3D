#include "collision/shapes.h"

#include <raylib/raylib.h>

Vector3 to_rl(const fm::vec3 a) { return {a.x, a.y, a.z};};
fm::vec3 to_fm(const Vector3& a) { return {a.x, a.y, a.z};}

Color g_collision_color_non = WHITE;
Color g_collision_color_hit = RED;

void SphereShape::draw()
{
	Color color = m_collision_count <= 0 ? g_collision_color_non : g_collision_color_hit;
	DrawSphereWires(to_rl(m_position), m_radius, 12, 12, color);
}

void AABBShape::draw()
{
	Color color = m_collision_count <= 0 ? g_collision_color_non : g_collision_color_hit;
	DrawCubeWiresV(to_rl(m_position),
		to_rl(m_size), color);
}

void ModelShape::draw()
{
	Color color = m_collision_count <= 0 ? g_collision_color_non : g_collision_color_hit;
	DrawModelWiresEx(m_model, to_rl(m_position), to_rl(m_rotation), 0.f, to_rl(m_scale), color);
}

