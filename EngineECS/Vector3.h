#pragma once

#include <memory>
#include<ostream>

#include "private.h"

namespace engineECS
{
	struct Vector3 final
	{
	public:
		float x;
		float y;
		float z;

		Vector3() : x(0), y(0), z(0) {};
		Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) {};
		Vector3(Vector3&& other) : x(other.x), y(other.y), z(other.z) {};
		Vector3& operator=(const Vector3& other) 
		{ 
			std::memcpy(this, &other, sizeof(Vector3)); 
			return *this; 
		};
		Vector3& operator=(Vector3&& other) 
		{ 
			std::memcpy(this, &other, sizeof(Vector3)); 
			return *this; 
		};
		Vector3 operator+(const Vector3& other) 
		{ 
			return Vector3(x + other.x, y + other.y, z + other.z);
		};
		Vector3 operator-(const Vector3& other)
		{
			return Vector3(x - other.x, y - other.y, z - other.z);
		};
		Vector3& operator+=(const Vector3& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		};
		Vector3& operator-=(const Vector3& other)
		{
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		};
		Vector3 operator*(const float other)
		{
			return Vector3(x * other, y * other, z * other);
		};
		Vector3 operator/(const float other)
		{
			return Vector3(x / other, y / other, z / other);
		};
		Vector3& operator*=(const float other)
		{
			x *= other;
			y *= other;
			z *= other;
			return *this;
		};
		Vector3& operator/=(const float other)
		{
			x /= other;
			y /= other;
			z /= other;
			return *this;
		};
		Vector3(const float value) : x(value), y(value), z(value) {};
		Vector3(const float inX, const float inY, const float inZ) : x(inX), y(inY), z(inZ) {};
	};

	static std::ostream& operator<<(std::ostream& LHS, const Vector3& RHS)
	{
		return LHS << "x: " << RHS.x << ", y: " << RHS.y << ", z: " << RHS.z;
	}
}