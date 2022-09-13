#pragma once
#include "collision/shapes.h"
#include "collision/bvh.h"

#include <raylib/raylib.h>

#include <vector>
class App
{
public:
	static void init();

	static Camera3D get_camera() { return m_camera; }
private:
	static void main_loop();
	static void update(float dt);
	static void detect_collisions();

	static Camera3D m_camera;
	static std::vector<Shape*> m_shapes;
	static Shape* m_selected_shape;

	static BVH m_dynamic_bvh;
};