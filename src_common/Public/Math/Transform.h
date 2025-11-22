#pragma once

#include "Vec3.h"
#include "Quaternion.h"

struct Transform {
	Vec3 position;
	Quaternion rotation;
	Vec3 scale;

	operator Mat4() const;
};

#include "Mat4.h"
inline Transform::operator Mat4() const {
	Mat4 scaleMat = scale.asScaleMat();
	Mat4 rotMat = rotation;
	Mat4 translationMat = position.asTranslateMat();

	return translationMat * rotMat * scaleMat;
}