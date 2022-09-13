#include "app.h"
#include "collision/functions.h"
#include "collision/bvh.h"

#include <raylib/rlgl.h>

#include <chrono>
#include <iostream>
#include <thread>

Camera3D App::m_camera = {0};
std::vector<Shape*> App::m_shapes = {};
Shape* App::m_selected_shape = nullptr;
BVH App::m_dynamic_bvh = {};

void App::init()
{
	InitWindow(1280, 720, "Collision 3D");
	SetTargetFPS(149);

	m_camera.position = { 0.f, 10.0f, 10.f };
	m_camera.target = { 0.0f, 0.0f, 0.0f };
	m_camera.up = { 0.0f, 1.0f, 0.0f };
	m_camera.fovy = 45.f;
	m_camera.projection = CAMERA_PERSPECTIVE;
	SetCameraMode(m_camera, CAMERA_FREE);

	m_shapes.push_back(new SphereShape({0.f,0.f,0.f}, 2.f));
	m_shapes.push_back(new SphereShape({0.f, 3.f, 0.f}, 2.f));

	m_shapes.push_back(new AABBShape({5.f,0.f,0.f}, {2.f, 2.f, 2.f}));
	m_shapes.push_back(new AABBShape({5.f,3.f,0.f}, {2.f, 2.f, 2.f}));

	m_shapes.push_back(new ModelShape("assets/models/fox.gltf",{10.f,1.f,0.f}, {0.02f, 0.02f, 0.02f}, {0.f,0.f,1.f}));
	m_shapes.push_back(new ModelShape("assets/models/fox.gltf",{50.f,1.f,0.f}, {0.02f, 0.02f, 0.02f}, {0.f,0.f,1.f}));

	for(int i = 0; i < 10; i++)
		m_shapes.push_back(new ModelShape("assets/models/fox.gltf",{12.f + i,1.f + (rand()%10)*0.1f,0.f}, {0.02f, 0.02f, 0.02f}, {0.f,0.f,1.f}));

	for(Shape* shape : m_shapes)
	{
		shape->calculate_aabb();

		m_dynamic_bvh.add_shape(shape);
	}

	m_dynamic_bvh.generate();

	main_loop();

	CloseWindow();
}

void App::main_loop()
{
	float delta_time = 0.0001f;
	while(!WindowShouldClose())
	{
		auto start = std::chrono::steady_clock::now();
		
		update(delta_time);
		
		auto end = std::chrono::steady_clock::now();
		std::chrono::duration<float> elapsed_chrono_time = end - start;
		delta_time = elapsed_chrono_time.count();
	}
}

void App::update(float dt)
{
	printf("%f\n",dt);
	m_dynamic_bvh.generate();

	UpdateCamera(&m_camera);

	BeginDrawing();
	{

		ClearBackground(BLACK);

		if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			Shape* selected_shape = nullptr;
			float distance = INFINITY;
			for(Shape* shape : m_shapes)
			{
				fm::Ray ray;
				ray.origin = { m_camera.position.x, m_camera.position.y, m_camera.position.z };
				Ray mouse_ray = GetMouseRay(GetMousePosition(), m_camera);
				ray.direction = {mouse_ray.direction.x, mouse_ray.direction.y, mouse_ray.direction.z };
				if(shape->hits_ray(ray, distance))
					selected_shape = shape;
			}
			m_selected_shape = selected_shape;
		}

		if(m_selected_shape)
		{
			if(IsKeyDown(KEY_UP))
				m_selected_shape->m_position.y += dt;
			if(IsKeyDown(KEY_DOWN))
				m_selected_shape->m_position.y -= dt;
			if(IsKeyDown(KEY_LEFT))
				m_selected_shape->m_position.x -= dt;
			if(IsKeyDown(KEY_RIGHT))
				m_selected_shape->m_position.x += dt;
		}

		//draw_guizmo();

		BeginMode3D(m_camera);
		{
			Vector3 origin = {0,0,0};
			DrawLine3D(origin, {50,0,0}, LIGHTGRAY);
			DrawLine3D(origin, {0,50,0}, LIGHTGRAY);
			DrawLine3D(origin, {0,0,50}, LIGHTGRAY);

			detect_collisions();

			m_dynamic_bvh.draw();

			for(Shape* shape : m_shapes)
			{
				shape->calculate_aabb();
				shape->draw();
				shape->reset_collisions();
			}
		}
		EndMode3D();
	}
	EndDrawing();
}

void App::detect_collisions()
{
	for(size_t i = 0; i < m_shapes.size(); i++)
	{
		for(size_t j = i+1; j < m_shapes.size(); j++)
		{
			ShapeType type0 = m_shapes[i]->get_type();
			ShapeType type1 = m_shapes[j]->get_type();

			if(type0 == ShapeType::Sphere && type1 == ShapeType::Sphere)
				detect_collision(static_cast<SphereShape*>(m_shapes[i]), static_cast<SphereShape*>(m_shapes[j]));

			else if(type0 == ShapeType::AABB && type1 == ShapeType::AABB)
				detect_collision(static_cast<AABBShape*>(m_shapes[i]), static_cast<AABBShape*>(m_shapes[j]));

			else if(type0 == ShapeType::AABB && type1 == ShapeType::Sphere)
				detect_collision(static_cast<AABBShape*>(m_shapes[i]), static_cast<SphereShape*>(m_shapes[j]));
			else if(type0 == ShapeType::Sphere && type1 == ShapeType::AABB)
				detect_collision(static_cast<AABBShape*>(m_shapes[j]), static_cast<SphereShape*>(m_shapes[i]));
			
			else if(type0 == ShapeType::Model && type1 == ShapeType::Model)
				detect_collision(static_cast<ModelShape*>(m_shapes[i]), static_cast<ModelShape*>(m_shapes[j]));

			else if(type0 == ShapeType::Model && type1 == ShapeType::Sphere)
				detect_collision(static_cast<ModelShape*>(m_shapes[i]), static_cast<SphereShape*>(m_shapes[j]));
			else if(type0 == ShapeType::Sphere && type1 == ShapeType::Model)
				detect_collision(static_cast<ModelShape*>(m_shapes[j]), static_cast<SphereShape*>(m_shapes[i]));
		
			else if(type0 == ShapeType::Model && type1 == ShapeType::AABB)
				detect_collision(static_cast<ModelShape*>(m_shapes[i]), static_cast<AABBShape*>(m_shapes[j]));
			else if(type0 == ShapeType::AABB && type1 == ShapeType::Model)
				detect_collision(static_cast<ModelShape*>(m_shapes[j]), static_cast<AABBShape*>(m_shapes[i]));
		}
	}
}