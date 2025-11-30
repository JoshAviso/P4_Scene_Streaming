#pragma once

#include <glm/glm.hpp>
#include <String.hpp>
#include <cmath>

union Vec4;
struct Mat4;

union Vec3 {
	struct { float x, y, z; };
	float data[3];

	Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
	Vec3(float _x) : Vec3(_x, _x, _x) {};
	operator Vec4() const;
	operator String() const;
	Vec3 operator+ (const Vec3& vec) const {
		return Vec3(x + vec.x, y + vec.y, z + vec.z);
	}
	Vec3& operator+= (const Vec3& vec) {
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}
	Vec3 operator* (const float scalar) {
		return Vec3(x * scalar, y * scalar, z * scalar);
	}
	Vec3& operator*= (const float scalar) {
		x *= scalar; y *= scalar; z *= scalar; return *this;
	}
	Vec3 cross(const Vec3& b) const {
		return Vec3(
			y * b.z - z * b.y,
			z * b.x - x * b.z,
			x * b.y - y * b.x
		);
	}
	
	Mat4 asScaleMat() const;
	Mat4 asTranslateMat() const;

	operator glm::vec3() const {
		return glm::vec3(x, y, z);
	}
	Vec3 normalized() const {
		float mag = magnitude();
		if (mag == 0.f) return Vec3(x, y, z);
		return Vec3(x / mag, y / mag, z / mag);
	}
	Vec3& normalize() {
		float mag = magnitude();
		if (mag == 0.f) return *this;
		x /= mag; y /= mag; z /= mag;
		return *this;
	}
	float sqrMagnitude() const{
		return x * x + y * y + z * z;
	}
	float magnitude() const {
		float sq = sqrMagnitude();
		if (sq == 0.f) return 0.f;
		return sqrt(sq);
	}
};

#include "Vec4.h"
inline Vec3::operator Vec4() const {
	return Vec4({ x, y, z, 1.f });
};

inline Vec3::operator String() const {
	return String("(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")");
}

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