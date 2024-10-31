#pragma once

#include "raylib.h"
#include <cmath>

////////////////////////////////////////////////////////////////////////////////
// Vector3
////////////////////////////////////////////////////////////////////////////////

struct vec3
{
	// data 
	float x{0.0f};
	float y{0.0f};
	float z{0.0f};

	// constructors
	constexpr vec3() = default;
	constexpr vec3(float x, float y, float z) : x(x), y(y), z(z) {};
	constexpr explicit vec3(float v) : x(v), y(v), z(v) {};


	// operators

	constexpr vec3 operator+(const vec3& other) const { return { x + other.x, y + other.y, z + other.z }; }
	constexpr vec3 operator-(const vec3& other) const { return { x - other.x, y - other.y, z - other.z }; }
	constexpr vec3 operator*(float scalar) const { return { x * scalar, y * scalar, z * scalar }; }
	constexpr vec3 operator/(float scalar) const { return { x / scalar, y / scalar, z / scalar }; }
	constexpr vec3 operator*(const vec3& other) const { return { x * other.x, y * other.y, z * other.z }; }
	constexpr vec3 operator/(const vec3& other) const { return { x / other.x, y / other.y, z / other.z }; }
	constexpr vec3 operator+=(const vec3& other) { return *this = *this + other; }
	constexpr vec3 operator-=(const vec3& other) { return *this = *this - other; }

	// spaceship operator
	constexpr bool operator==(const vec3& other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}
	constexpr bool operator!=(const vec3& other) const
	{
		return !(*this == other);
	}

	// dot product
	constexpr float dot(const vec3& other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}
	// lengthSquared
	constexpr float lengthSquared() const
	{
		return dot(*this);
	}
	// length
	float length() const
	{
		return std::sqrt(lengthSquared());
	}
	// normalize
	vec3 norm() const
	{
		return *this / length();
	}

	// conversion to raylib Vector3
	operator Vector3() const { return { x, y, z }; }
};

struct vec4
{
	float x{0.0f};
	float y{0.0f};
	float z{0.0f};
	float w{0.0f};

	// constructors
	constexpr vec4() = default;
	constexpr vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};
	constexpr explicit vec4(float v) : x(v), y(v), z(v), w(v) {};


	// operators

	constexpr vec4 operator+(const vec4& other) const { return { x + other.x, y + other.y, z + other.z, w + other.w }; }
	constexpr vec4 operator-(const vec4& other) const { return { x - other.x, y - other.y, z - other.z, w - other.w }; }
	constexpr vec4 operator*(float scalar) const { return { x * scalar, y * scalar, z * scalar, w * scalar }; }
	constexpr vec4 operator/(float scalar) const { return { x / scalar, y / scalar, z / scalar, w / scalar }; }
	constexpr vec4 operator*(const vec4& other) const { return { x * other.x, y * other.y, z * other.z, w * other.w }; }
	constexpr vec4 operator/(const vec4& other) const { return { x / other.x, y / other.y, z / other.z, w / other.w }; }
	constexpr vec4 operator+=(const vec4& other) { return *this = *this + other; }
	constexpr vec4 operator-=(const vec4& other) { return *this = *this - other; }

	// spaceship operator
	constexpr bool operator==(const vec4& other) const
	{
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}
	constexpr bool operator!=(const vec4& other) const
	{
		return !(*this == other);
	}
	// dot product
	constexpr float dot(const vec4& other) const
	{
		return x * other.x + y * other.y + z * other.z + w * other.w;
	}
	// lengthSquared
	constexpr float lengthSquared() const
	{
		return dot(*this);
	}
	// length
	float length() const
	{
		return std::sqrt(lengthSquared());
	}
	// normalize
	vec4 norm() const
	{
		return *this / length();
	}

	// conversion to raylib Vector4
	operator Vector4() const { return { x, y, z, w }; }
	// conversion to raylib Color
	operator Color() const { return { (unsigned char)(x * 255), (unsigned char)(y * 255), (unsigned char)(z * 255), (unsigned char)(w * 255) }; }
};

// free functions for vec3$
float	length	(const vec3& v);
vec3	norm	(const vec3& v);




struct Bbox
{
	vec3 min;
	vec3 max;

	void draw(Color color) const;
};


struct Domain
{
	float height;
	float width;
	float depth;

	Bbox get_bbox() const;

	vec3 get_extents() const;
	float get_volume() const;

	void draw(Color color) const;

};

static constexpr vec3 a = { 1, 2, 3 };
static constexpr vec3 b = { 4, 5, 6 };
static constexpr vec3 c = { 4, 5, 6 };

static_assert(a + b == vec3{ 5, 7, 9 }				,"FAILED");
static_assert(a - b == vec3{ -3, -3, -3 }			,"FAILED");
static_assert(a * 2 == vec3{ 2, 4, 6 }				,"FAILED");
static_assert(a / 2 == vec3{ 0.5, 1, 1.5 }			,"FAILED");
static_assert(a * b == vec3{ 4, 10, 18 }			,"FAILED");
static_assert(a / b == vec3{ 0.25f, 0.4f, 0.5f }		,"FAILED");
static_assert(a != b ,"FAILED");
static_assert(c == b ,"FAILED");