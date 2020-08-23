#ifndef MATH_H
#define MATH_H

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <imgui/imgui.h>

#include <cstdint>

class Vector2 {
public:

	Vector2() = default;

	Vector2(const float scalar) :
		x(scalar), y(scalar) {}
	Vector2(const int scalar) :
		x(static_cast<float>(scalar)), y(static_cast<float>(scalar)) {}
	Vector2(const uint32_t scalar) :
		x(static_cast<float>(scalar)), y(static_cast<float>(scalar)) {}
	Vector2(const double scalar) :
		x(static_cast<float>(scalar)), y(static_cast<float>(scalar)) {}
	
	Vector2(const float _x, const float _y) :
		x(_x), y(_y) {}
	Vector2(const int _x, const int _y) :
		x(static_cast<float>(_x)), y(static_cast<float>(_y)) {}
	Vector2(const uint32_t _x, const uint32_t _y) :
		x(static_cast<float>(_x)), y(static_cast<float>(_y)) {}
	Vector2(const double _x, const double _y) :
		x(static_cast<float>(_x)), y(static_cast<float>(_y)) {}

	Vector2(const Vector2&) = default;
	Vector2(const ImVec2& other)    : x(other.x), y(other.y) {}
	Vector2(const glm::vec2& other) : x(other.x), y(other.y) {}
	
	operator ImVec2()    const { return { x, y }; }
	operator glm::vec2() const { return { x, y }; }

	Vector2& operator=(const Vector2&) = default;

	Vector2 operator+(const float scalar)   const { return { x + scalar, y + scalar }; }
	Vector2 operator+(const Vector2& other) const { return { x + other.x, y + other.y }; }
	Vector2& operator+=(const float scalar)       { *this = *this + scalar; return *this; }
	Vector2& operator+=(const Vector2& other)     { *this = *this + other; return *this; }

	Vector2 operator-(const float scalar)   const { return { x - scalar, y - scalar }; }
	Vector2 operator-(const Vector2& other) const { return { x - other.x, y - other.y }; }
	Vector2& operator-=(const float scalar)       { *this = *this - scalar; return *this; }
	Vector2& operator-=(const Vector2& other)     { *this = *this - other; return *this; }

	Vector2 operator*(const float scalar)   const { return { x * scalar, y * scalar }; }
	Vector2& operator*=(const float scalar)       { *this = *this * scalar; return *this; }
	
	Vector2 operator/(const float scalar)   const { return { x / scalar, y / scalar }; }
	Vector2& operator/=(const float scalar)       { *this = *this / scalar; return *this; }

	static float length(const Vector2& vec)  { return glm::length(glm::vec2(vec)); }
	static float length2(const Vector2& vec) { return glm::length2(glm::vec2(vec)); }
	
	float x = 0.0f, y = 0.0f;
};

#endif
