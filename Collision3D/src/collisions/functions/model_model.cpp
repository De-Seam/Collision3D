#include "collision/functions.h"

bool sat_intersect(ModelShape* a, ModelShape* b)
{
	int vert_count_a = a->m_model.meshes[0].vertexCount;
	int vert_count_b = a->m_model.meshes[0].vertexCount;

	for(int i = 0; i < vert_count_a; i+=3)
	{
		fm::vec3 current = {a->m_model.meshes[0].vertices[i], a->m_model.meshes[0].vertices[i+1], a->m_model.meshes[0].vertices[i+2]};
		int index = (i+1)%vert_count_a;
		fm::vec3 next = {a->m_model.meshes[0].vertices[index], a->m_model.meshes[0].vertices[index+1], a->m_model.meshes[0].vertices[index+2]};
		fm::vec3 edge = next - current;
		edge *= a->m_scale;
		//edge += a->m_position;

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
			v*= a->m_scale;
			v+=a->m_position;
			float proj = axis.dot(v);

			if(proj < a_min_proj)
				a_min_proj = proj;
			if(proj > a_max_proj)
				a_max_proj = proj;
		}

		for(size_t j = 0; j < vert_count_b; j+=3)
		{
			fm::vec3 v = {b->m_model.meshes[0].vertices[j], b->m_model.meshes[0].vertices[j+1], b->m_model.meshes[0].vertices[j+2]};
			v*=b->m_scale;
			v+=b->m_position;
			float proj = axis.dot(v);

			if(proj < b_min_proj)
				b_min_proj = proj;
			if(proj > b_max_proj)
				b_max_proj = proj;
		}

		if(a_max_proj < b_min_proj || a_min_proj > b_max_proj)
		{
			return false; //No intersection
		}
	}
	//At this point we're sure there's an intersection
	return true;
}

fm::vec3 find_furthest_point_model(ModelShape* model, fm::vec3 direction)
{
	int vert_count = model->m_model.meshes[0].vertexCount;
	fm::vec3 max_point;
	float max_distance = -INFINITY;

	for(int i = 0; i < vert_count; i+=3)
	{
		fm::vec3 vertex = {model->m_model.meshes[0].vertices[i], model->m_model.meshes[0].vertices[i+1], model->m_model.meshes[0].vertices[i+2]};
		vertex *= model->m_scale;
		vertex += model->m_position;
		float distance = vertex.dot(direction);
		if(distance > max_distance)
		{
			max_distance = distance;
			max_point = vertex;
		}
	}

	return max_point;
}

fm::vec3 find_furthest_point_sphere(SphereShape* sphere, fm::vec3 direction)
{
	return sphere->m_position + fm::normalize(direction) * sphere->m_radius;
}

fm::vec3 support_model_model(ModelShape* a, ModelShape* b, fm::vec3 direction)
{
	return find_furthest_point_model(a, direction) - find_furthest_point_model(b, -direction);
}

fm::vec3 support_model_sphere(ModelShape* a, SphereShape* b, fm::vec3 direction)
{
	return find_furthest_point_model(a, direction) - find_furthest_point_sphere(b, -direction);
}

#include <array>

struct Simplex {
	private:
		std::array<fm::vec3, 4> m_points;
		unsigned m_size;

	public:
		Simplex()
			: m_points({ 0, 0, 0, 0 })
			, m_size(0)
		{}

		Simplex& operator=(std::initializer_list<fm::vec3> list) {
			for (auto v = list.begin(); v != list.end(); v++) {
				m_points[std::distance(list.begin(), v)] = *v;
			}
			m_size = list.size();

			return *this;
		}

		void push_front(fm::vec3 point) {
			m_points = { point, m_points[0], m_points[1], m_points[2] };
			m_size = std::min(m_size + 1, 4u);
		}

		fm::vec3& operator[](unsigned i) { return m_points[i]; }
		unsigned size() const { return m_size; }

		auto begin() const { return m_points.begin(); }
		auto end()   const { return m_points.end() - (4 - m_size); }
	};

bool same_direction(const fm::vec3& direction, const fm::vec3 a)
{
	return direction.dot(a) > 0.f;
}

bool line(Simplex& simplex, fm::vec3& direction)
{
	fm::vec3 a = simplex[0];
	fm::vec3 b = simplex[1];

	fm::vec3 ab = b-a;
	fm::vec3 ao = -a;

	if(same_direction(ab, ao))
	{
		direction = ab.cross(ao).cross(ab);
	}
	else
	{
		simplex = {a};
		direction = ao;
	}

	return false;
}

bool triangle(Simplex& simplex, fm::vec3& direction)
{
	fm::vec3 a = simplex[0];
	fm::vec3 b = simplex[1];
	fm::vec3 c = simplex[2];

	fm::vec3 ab = b - a;
	fm::vec3 ac = c - a;
	fm::vec3 ao = -a;

	fm::vec3 abc = ab.cross(ac);

	if(same_direction(abc.cross(ac), ao))
	{
		if(same_direction(ac, ao))
		{
			simplex = { a, c};
			direction = ac.cross(ao).cross(ac);
		}
		else
		{
			return line(simplex = {a,b}, direction);
		}
	}
	else
	{
		if(same_direction(ab.cross(abc), ao))
		{
			return line(simplex = {a,b}, direction);
		}
		else
		{
			if(same_direction(abc,ao))
			{
				direction = abc;
			}
			else
			{
				simplex = {a,c,b};
				direction = -abc;
			}
		}
	}
	return false;
}

bool tetrahedron(Simplex& simplex, fm::vec3& direction)
{
	fm::vec3 a = simplex[0];
	fm::vec3 b = simplex[1];
	fm::vec3 c = simplex[2];
	fm::vec3 d = simplex[3];

	fm::vec3 ab = b - a;
	fm::vec3 ac = c - a;
	fm::vec3 ad = d - a;
	fm::vec3 ao = -a;

	fm::vec3 abc = ab.cross(ac);
	fm::vec3 acd = ac.cross(ad);
	fm::vec3 adb = ad.cross(ab);

	if(same_direction(abc,ao))
	{
		return triangle(simplex = {a,b,c}, direction);
	}
	if(same_direction(acd,ao))
	{
		return triangle(simplex = {a,c,d}, direction);
	}
	if(same_direction(adb,ao))
	{
		return triangle(simplex = {a,d,b}, direction);
	}

	return true;
}

bool next_simplex(Simplex& simplex, fm::vec3& direction)
{
	switch(simplex.size())
	{
	case 2: 
		return line(simplex, direction);
		break;
	case 3:
		return triangle(simplex, direction);
		break;
	case 4:
		return tetrahedron(simplex, direction);
		break;
	}

	return false;
}

bool gjk_intersect(ModelShape* a, ModelShape* b)
{
	Simplex simplex;

	fm::vec3 s = support_model_model(a, b, fm::vec3{1.f,0.f,0.f});
	simplex.push_front(s);
	fm::vec3 direction = -s;

	while(true)
	{
		s = support_model_model(a, b, direction);
		if(s.dot(direction) <= 0.f) //No collision
		{
			return false;
		}

		simplex.push_front(s);

		if(next_simplex(simplex, direction))
			return true;
	}
}

void detect_collision(ModelShape* a, ModelShape* b)
{
	//bool intersect = sat_intersect(a,b);
	bool intersect = gjk_intersect(a,b);
	if(intersect)
	{
		a->add_collision();
		b->add_collision();
	}
}


void detect_collision(ModelShape* model, SphereShape* sphere)
{
	//GJK

	Simplex simplex;

	fm::vec3 s = support_model_sphere(model, sphere, fm::vec3{1.f,0.f,0.f});
	simplex.push_front(s);
	fm::vec3 direction = -s;

	while(true)
	{
		s = support_model_sphere(model, sphere, direction);
		if(s.dot(direction) <= 0.f) //No collision
		{
			return;
		}

		simplex.push_front(s);

		if(next_simplex(simplex, direction))
		{
			model->add_collision();
			sphere->add_collision();
			return;
		}
	}
}