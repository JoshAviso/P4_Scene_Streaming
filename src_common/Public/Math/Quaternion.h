#pragma once

#include <Math/Constants.h>
#include <Math/Vec3.h>

struct Mat4;

union Quaternion {
	struct { float w, x, y, z; };
	float data[4];

	Quaternion() : w(1.f), x(0.f), y(0.f), z(0.f) {};
	Quaternion(float _w, float _x, float _y, float _z) : w(_w), x(_x), y(_y), z(_z) {};
	Quaternion(float degrees, Vec3 axis) {
		float rad = degrees * PI / 180.f;
		float halfAngle = rad * 0.5f;
		float s = sinf(halfAngle);
		w = cosf(halfAngle);
		x = axis.x * s;
		y = axis.y * s;
		z = axis.z * s;
	}
	operator Mat4() const;
	Quaternion operator* (const Quaternion& q) const {
		Quaternion r(
			w * q.w - x * q.x - y * q.y - z * q.z,
			w * q.x + x * q.w + y * q.z - z * q.y,
			w * q.y - x * q.z + y * q.w + z * q.x,
			w * q.z + x * q.y - y * q.x + z * q.w
		);
		return r;
	}
	Vec3 operator* (const Vec3& vec) const {
		Quaternion p(0.f, vec.x, vec.y, vec.z);
		Quaternion inv = conjugate();
		Quaternion n = normalized();

		Quaternion res = (n * p) * inv;
		return Vec3(res.x, res.y, res.z);
	};
	Quaternion conjugate() const {
		return Quaternion(w, -x, -y, -z);
	}
	float sqMagnitude() const {
		return w * w + x * x + y * y + z * z;
	}
	float magnitude() const {
		float sq = sqMagnitude();
		if (sq == 0.f) return 0.f;
		return sqrt(sq);
	}
	Quaternion& normalize() {
		float mag = magnitude();
		if (mag == 0.f) return *this;
		w /= mag; x /= mag; y /= mag; z /= mag;
		return *this;
	}
	Quaternion normalized() const {
		float mag = magnitude();
		if (mag == 0.f) return Quaternion(w, x, y, z);
		return Quaternion(w / mag, x / mag, y / mag, z / mag);
	}
	static const Quaternion& Identity() { static const Quaternion q; return q; }
};

#include "Mat4.h"
inline Quaternion::operator Mat4() const {
	Mat4 mat(1.f);

	// Precompute terms
	float xx = x * x;
	float yy = y * y;
	float zz = z * z;
	float xy = x * y;
	float xz = x * z;
	float yz = y * z;

	mat.data[0][0] = 1.f - 2.f * (yy + zz);
	mat.data[0][1] = 2.f * (xy - w * z);
	mat.data[0][2] = 2.f * (xz + w * y);
	mat.data[0][3] = 0.f;

	mat.data[1][0] = 2.f * (xy + w * z);
	mat.data[1][1] = 1.f - 2.f * (xx + zz);
	mat.data[1][2] = 2.f * (yz - w * x);
	mat.data[1][3] = 0.f;

	mat.data[2][0] = 2.f * (xz - w * y);
	mat.data[2][1] = 2.f * (yz + w * x);
	mat.data[2][2] = 1.f - 2.f * (xx + yy);
	mat.data[2][3] = 0.f;

	mat.data[3][0] = 0.f;
	mat.data[3][1] = 0.f;
	mat.data[3][2] = 0.f;
	mat.data[3][3] = 1.f;

	return mat;
}