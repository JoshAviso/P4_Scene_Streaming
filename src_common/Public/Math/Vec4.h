#pragma once

union Vec3;

union Vec4 {
	struct { float x, y, z, a; };
	float data[4];

	operator Vec3() const;
};

#include "Vec3.h"
inline Vec4::operator Vec3() const {
	return Vec3( x, y, z );
}