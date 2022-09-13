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
	DrawCubeWiresV(to_rl(m_position), to_rl(m_size), color);
}

void ModelShape::draw()
{
	fm::mat4 translation_matrix = fm::translate(fm::mat4{1.f}, m_position);
	fm::mat4 scale_matrix = fm::scale(fm::mat4{1.f}, m_scale);
	//fm::mat4 rotate_matrix = fm::rotate(fm::mat4{1.f}, 0.f, m_rotation);

	fm::mat4 model_matrix = scale_matrix * translation_matrix;

	Matrix rl_transform;

	rl_transform.m0 = model_matrix[0][0];
	rl_transform.m1 = model_matrix[0][1];
	rl_transform.m2 = model_matrix[0][2];
	rl_transform.m3 = model_matrix[0][3];
	
	rl_transform.m4 = model_matrix[1][0];
	rl_transform.m5 = model_matrix[1][1];
	rl_transform.m6 = model_matrix[1][2];
	rl_transform.m7 = model_matrix[1][3];
	
	rl_transform.m8 = model_matrix[2][0];
	rl_transform.m9 = model_matrix[2][1];
	rl_transform.m10 = model_matrix[2][2];
	rl_transform.m11 = model_matrix[2][3];
	
	rl_transform.m12 = model_matrix[3][0];
	rl_transform.m13 = model_matrix[3][1];
	rl_transform.m14 = model_matrix[3][2];
	rl_transform.m15 = model_matrix[3][3];

	m_model.transform = rl_transform;

	Color color = m_collision_count <= 0 ? g_collision_color_non : g_collision_color_hit;
	//DrawModelWiresEx(m_model, to_rl(m_position), to_rl(m_rotation), 0.f, to_rl(m_scale), color);
	DrawModelWires(m_model, {0,0,0}, 1.f, color);

	//DrawMesh(m_model.meshes[0], m_model.materials[0], m_model.transform);
}

