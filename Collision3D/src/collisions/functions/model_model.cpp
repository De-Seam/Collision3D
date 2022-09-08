#include "collision/functions.h"

void detect_collision(ModelShape* a, ModelShape* b)
{
	int vert_count_a = a->m_model.meshes[0].vertexCount;
	int vert_count_b = a->m_model.meshes[0].vertexCount;

	for(int i = 0; i < vert_count_a; i+=3)
	{
		fm::vec3 current = {a->m_model.meshes[0].vertices[i], a->m_model.meshes[0].vertices[i+1], a->m_model.meshes[0].vertices[i+2]};
		int index = (i+1)%vert_count_a;
		fm::vec3 next = {a->m_model.meshes[0].vertices[index], a->m_model.meshes[0].vertices[index+1], a->m_model.meshes[0].vertices[index+2]};
		fm::vec3 edge = next - current;

		fm::vec3 axis;
		axis[0] = -edge[1];
		axis[1] = edge[0];

		float a_max_proj = -INFINITY;
		float a_min_proj = INFINITY;
		float b_max_proj = -INFINITY;
		float b_min_proj = INFINITY;

		for(size_t j = 0; j < vert_count_a; j+=3)
		{
			fm::vec3 v = {a->m_model.meshes[0].vertices[j], a->m_model.meshes[0].vertices[j+1], a->m_model.meshes[0].vertices[j+2]};
			float proj = axis.dot(v);

			if(proj < a_min_proj)
				a_min_proj = proj;
			if(proj > a_max_proj)
				a_max_proj = proj;
		}

		for(size_t j = 0; j < vert_count_b; j+=3)
		{
			fm::vec3 v = {b->m_model.meshes[0].vertices[j], b->m_model.meshes[0].vertices[j+1], b->m_model.meshes[0].vertices[j+2]};
			float proj = axis.dot(v);

			if(proj < a_min_proj)
				a_min_proj = proj;
			if(proj > a_max_proj)
				a_max_proj = proj;
		}

		if(a_max_proj < b_min_proj || a_min_proj > b_max_proj)
		{
			a->add_collision();
			b->add_collision();
		}
	}
}