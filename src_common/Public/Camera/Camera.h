#pragma once

#include <Math/Mat4.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Math/Vec3.h>
#include <Math/Quaternion.h>

class Camera {
public:
	struct Projection {
		float zNear = 0.01f;
		float zFar = 100.f;
		Projection() {};
		Projection(float _zNear, float _zFar) : zNear(_zNear), zFar(_zFar) {};
		virtual Mat4 GetMat() = 0;
	};;
	struct PerspectiveProjection;
	struct OrthoProjection;

	Projection* projection;
	Vec3 position = Vec3(0.f);
	Quaternion rotation = Quaternion::Identity();

	Mat4 GetViewMat();
	Mat4 GetProjection() { 
		if (projection == nullptr) return Mat4(1.f);
		return projection->GetMat(); 
	};
};

struct Camera::PerspectiveProjection : Camera::Projection {
	float fov_y;
	float aspect_ratio_x_by_y;
	PerspectiveProjection(float _fov_y, float _aspect_ratio_x_by_y, float _zNear, float _zFar) :
		Projection(_zNear, _zFar), fov_y(_fov_y), aspect_ratio_x_by_y(_aspect_ratio_x_by_y) {}
	Mat4 GetMat() override { return glm::perspective(glm::radians(fov_y), aspect_ratio_x_by_y, zNear, zFar); }
};

struct Camera::OrthoProjection : Camera::Projection {
	float top;
	float right;
	float bottom;
	float left;
	OrthoProjection(float _left, float _right, float _bottom, float _top, float _zNear, float _zFar) :
		Projection(_zNear, _zFar), left(_left), right(_right), bottom(_bottom), top(_top) {}
	Mat4 GetMat() override { return glm::ortho(left, right, bottom, top, zNear, zFar); }
};