#pragma once 

#include "Vec3.h"
#include "Quaternion.h"
#include "Vec2.h"
#include <vector>

struct TempVertex {
	Vec3 position = Vec3(0.f);
	Vec3 normal = Vec3(0.f);
	Vec2 uv = { 0.f, 0.f };
};

struct TempObject {
	Vec3 position = Vec3(0.f);
	Vec3 rotation = Vec3(0.f);
	Vec3 scale = Vec3(1.f);

	std::vector<TempVertex> vertices;
};