#pragma once
#include "collision/shapes.h"

#include <raylib/raylib_o.h>

#include <vector>
class App
{
public:
	static void init();

private:
	static void main_loop();
	static void update(float dt);
	static void detect_collisions();

	static Camera3D m_camera;
	static std::vector<Shape*> m_shapes;
	static Shape* m_selected_shape;
};