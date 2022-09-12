#pragma once
#include "shapes.h"

bool collides(const fm::AABB& a, const fm::AABB& b);
void detect_collision(AABBShape*, AABBShape*);
void detect_collision(SphereShape*, SphereShape*);

void detect_collision(AABBShape*, SphereShape*);
//void detect_collision(SphereShape* a, AABBShape* b) { detect_collision(b, a);}

void detect_collision(ModelShape*, ModelShape*);

void detect_collision(ModelShape*, SphereShape*);