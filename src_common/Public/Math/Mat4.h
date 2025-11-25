#pragma once

#include <glm/glm.hpp>
#include <String.hpp>

union Vec4;

struct Mat4 {
	float data[4][4];
	
	Mat4(float diagonal) {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				for (int k = 0; k < 4; ++k) {
					data[i][j] = 0.f;
				}
			}
		}
		data[0][0] = diagonal;
		data[1][1] = diagonal;
		data[2][2] = diagonal;
		data[3][3] = diagonal;
	}
	
	Mat4 operator* (const Mat4& right) const {
		Mat4 mat(0.f);
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				for (int k = 0; k < 4; ++k) {
					mat.data[i][j] += data[i][k] * right.data[k][j];
				}
			}
		}
		return mat;
	}
	Vec4 operator* (const Vec4& vec) const;
	operator glm::mat4() const {
		glm::mat4 mat;
		mat[0] = glm::vec4(data[0][0], data[0][1], data[0][2], data[0][3]);
		mat[1] = glm::vec4(data[1][0], data[1][1], data[1][2], data[1][3]);
		mat[2] = glm::vec4(data[2][0], data[2][1], data[2][2], data[2][3]);
		mat[3] = glm::vec4(data[3][0], data[3][1], data[3][2], data[3][3]);
		return mat;
	}
	Mat4(const glm::mat4& glmMat) {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				data[i][j] = glmMat[i][j];
			}
		}
	}
	operator String() const {
		return "(" + 
			std::to_string(data[0][0]) + ", " + std::to_string(data[0][1]) + ", " + std::to_string(data[0][2]) + ", " + std::to_string(data[0][3]) + ", " +
			std::to_string(data[1][0]) + ", " + std::to_string(data[1][1]) + ", " + std::to_string(data[1][2]) + ", " + std::to_string(data[1][3]) + ", " +
			std::to_string(data[2][0]) + ", " + std::to_string(data[2][1]) + ", " + std::to_string(data[2][2]) + ", " + std::to_string(data[2][3]) + ", " +
			std::to_string(data[3][0]) + ", " + std::to_string(data[3][1]) + ", " + std::to_string(data[3][2]) + ", " + std::to_string(data[3][3]) + ")";
	}
};

#include "Vec4.h"
inline Vec4 Mat4::operator* (const Vec4& vec) const {
	Vec4 res;
	res.x = data[0][0] * vec.x + data[0][1] * vec.y + data[0][2] * vec.z + data[0][3] * vec.a;
	res.y = data[1][0] * vec.x + data[1][1] * vec.y + data[1][2] * vec.z + data[1][3] * vec.a;
	res.z = data[2][0] * vec.x + data[2][1] * vec.y + data[2][2] * vec.z + data[2][3] * vec.a;
	res.a = data[3][0] * vec.x + data[3][1] * vec.y + data[3][2] * vec.z + data[3][3] * vec.a;
	return res;
}