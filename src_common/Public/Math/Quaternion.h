#pragma once

struct Mat4;

union Quaternion {
	struct { float w, x, y, z; };
	float data[4];

	operator Mat4() const;
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