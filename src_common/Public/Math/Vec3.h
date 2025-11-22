#pragma once

#include <glm/glm.hpp>

union Vec4;
struct Mat4;

union Vec3 {
	struct { float x, y, z; };
	float data[3];

	operator Vec4() const;
	
	Mat4 asScaleMat() const;
	Mat4 asTranslateMat() const;

	operator glm::vec3() const {
		return glm::vec3(x, y, z);
	}
};

#include "Vec4.h"
inline Vec3::operator Vec4() const {
	return Vec4({ x, y, z, 1.f });
};

#include "Mat4.h"
inline Mat4 Vec3::asScaleMat() const {
	Mat4 mat(1.f);
	mat.data[0][0] = x;
	mat.data[1][1] = y;
	mat.data[2][2] = z;
	return mat;
};
inline Mat4 Vec3::asTranslateMat() const {
	Mat4 mat(1.f);
	mat.data[0][3] = x;
	mat.data[1][3] = y;
	mat.data[2][3] = z;
	return mat;
}