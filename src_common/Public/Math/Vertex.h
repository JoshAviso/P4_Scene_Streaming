#pragma once

#include "Vec3.h"
#include "Vec2.h"

struct Vertex {
	Vec3 position = Vec3(0.f);
	Vec3 normal = Vec3(0.f);
	Vec2 uv = {0.f, 0.f};
};