#pragma once

#include <String.hpp>

union Vec2 {
	struct { float x, y; };
	float data[2];

	operator String() const {
		return String("(" + std::to_string(x) + ", " + std::to_string(y) + ")");
	}

	Vec2 operator- (const Vec2& vec) const {
		return Vec2({ x - vec.x, y - vec.y });
	}
	Vec2 operator* (const float scalar) const {
		return Vec2({ x * scalar, y * scalar });
	}
	Vec2& operator*= (const float scalar) {
		x *= scalar; y *= scalar;
		return *this;
	}

	Vec2 normalize() {
		 float length = std::sqrt(x * x + y * y);
		 if (length > 0.0f) {
			 x /= length;
			 y /= length;
		 }
		 return *this;
	}
};