#pragma once

#include "Common.h"
#include <Math/Vec3.h>

class Color {
public:
    union {
        struct { float r, g, b, a; };
        struct { float R, G, B, A; };
        float data[4];
    };
    constexpr Color() noexcept : r(1.f), g(1.f), b(1.f), a(1.f) {}
    constexpr Color(float red, float green, float blue, float alpha = 1.f) noexcept
        : r(red), g(green), b(blue), a(alpha) {
    }
    operator Vec3() const {
        return Vec3({ r, g, b });
    }

    Color& operator=(const Color& other) {
        r = other.r;
        g = other.g;
        b = other.b;
        a = other.a;
        return *this;
    };
    bool operator==(const Color& other) const noexcept {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }
    bool operator!=(const Color& other) const noexcept {
        return !(*this == other);
    }
	static Color Lerp(const Color& first, const Color& second, float delta) {
        Color color;
        color.r = static_cast<uint8>(first.r + (second.r - first.r) * delta);
		color.g = static_cast<uint8>(first.g + (second.g - first.g) * delta);
		color.b = static_cast<uint8>(first.b + (second.b - first.b) * delta);
		color.a = static_cast<uint8>(first.a + (second.a - first.a) * delta);
		return color;
	}

    // Predefined Colors
    static const Color& White()     { static const Color white  (1.f, 1.f, 1.f, 1.f);   return white; };
    static const Color& Black()     { static const Color black  (0.f, 0.f, 0.f, 1.f);         return black; };
    static const Color& Red()       { static const Color red    (1.f, 0.f, 0.f, 1.f);       return red; };
    static const Color& Green()     { static const Color green  (0.f, 1.f, 0.f, 1.f);       return green; };
    static const Color& Blue()      { static const Color blue   (0.f, 0.f, 1.f, 1.f);       return blue; };
    static const Color& Yellow()    { static const Color yellow (1.f, 1.f, 0.f, 1.f);     return yellow; }
};