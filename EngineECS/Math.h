#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <iostream>

#include "Vector3.h"
#include "Matrix4.h"
#include <PxPhysicsAPI.h>
#include "Vector2.h"

static glm::vec3 vector3FromFFHtoGLM(const ffh::Vector3& toConvert)
{
	glm::vec3 result;
	result.x = toConvert.x;
	result.y = toConvert.y;
	result.z = toConvert.z;
	return result;
}
static glm::vec3 vector3FromPxtoGLM(const physx::PxVec3& toConvert)
{
	glm::vec3 result;
	result.x = toConvert.x;
	result.y = toConvert.y;
	result.z = toConvert.z;
	return result;
}
static glm::vec2 vector2FromFFHtoGLM(const ffh::Vector2& toConvert)
{
	glm::vec2 result;
	result.x = toConvert.x;
	result.y = toConvert.y;
	return result;
}
static glm::mat4 matrix4FromFFHtoGLM(const ffh::Matrix4& toConvert)
{
	return glm::mat4{
		toConvert.data[0][0],
		toConvert.data[0][1],
		toConvert.data[0][2],
		toConvert.data[0][3],
		toConvert.data[1][0],
		toConvert.data[1][1],
		toConvert.data[1][2],
		toConvert.data[1][3],
		toConvert.data[2][0],
		toConvert.data[2][1],
		toConvert.data[2][2],
		toConvert.data[2][3],
		toConvert.data[3][0],
		toConvert.data[3][1],
		toConvert.data[3][2],
		toConvert.data[3][3],
	};
}

static std::ostream& operator<<(std::ostream& out, const glm::mat4 matrix)
{
	out << matrix[0][0] << " " << matrix[1][0] << " " << matrix[2][0] << " " << matrix[3][0] << std::endl;
	out << matrix[0][1] << " " << matrix[1][1] << " " << matrix[2][1] << " " << matrix[3][1] << std::endl;
	out << matrix[0][2] << " " << matrix[1][2] << " " << matrix[2][2] << " " << matrix[3][2] << std::endl;
	out << matrix[0][3] << " " << matrix[1][3] << " " << matrix[2][3] << " " << matrix[3][3] << std::endl;
	return out;
}

static std::ostream& operator<<(std::ostream& out, const glm::quat quaternion)
{
	auto vec = glm::degrees(glm::eulerAngles(quaternion));
	return out << vec.x << "," << vec.y << "," << vec.z;
}

static std::ostream& operator<<(std::ostream& out, const glm::vec3 vector)
{
	return out << vector.x << "," << vector.y << "," << vector.z;
}