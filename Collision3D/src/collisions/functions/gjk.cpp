#include "collision/functions.h"

fm::vec3 support(Shape* a, Shape* b, fm::vec3 direction)
{
	return a->find_furthest_point(direction) - b->find_furthest_point(-direction);
}

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

bool colliding(Shape* a, Shape* b)
{
	if(!collides(a->m_aabb, b->m_aabb)) //Broadphase AABB check
		return false;

	Simplex simplex;

	fm::vec3 s = support(a, b, fm::vec3{1.f,0.f,0.f});
	simplex.push_front(s);
	fm::vec3 direction = -s;

	while(true)
	{
		s = support(a, b, direction);
		if(s.dot(direction) <= 0.f) //No collision
			return false;

		simplex.push_front(s);

		if(next_simplex(simplex, direction))
			return true;
	}
}