#pragma once

#include "Math.h"
#include "Vector3.h"
#include "Matrix4.h"
#include <map>
#include <vector>
#include <array>
#include "Vector2.h"

namespace engineECS
{
	struct SkeletalMesh
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;

		std::vector<glm::mat4> bindPoses;

		std::vector<std::array<int, 4>> bones;
		std::vector<std::array<float, 4>> weights;

		std::map<std::string, std::vector<std::vector<glm::mat4>>> animations;

		GLuint vao;
		std::array<GLuint, 5> buffers;
		bool uploaded : 1;

		GLsizei numVertices;

		void cleanup();

		void upload();

	private:
		SkeletalMesh(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uvs, const std::vector<glm::mat4>& bindPoses, const std::vector<std::array<int, 4>>& bones, const std::vector<std::array<float, 4>>& weights, const std::map<std::string, std::vector<std::vector<glm::mat4>>>& animations);
		SkeletalMesh(const std::vector<ffh::Vector3>& vertices, const std::vector<ffh::Vector3>& normals, const std::vector<ffh::Vector2>& uvs, const std::vector<ffh::Matrix4>& bindPoses, const std::vector<std::array<int, 4>>& bones, const std::vector<std::array<float, 4>>& weights, const std::map<std::string, std::vector<std::vector<ffh::Matrix4>>>& animations);
		SkeletalMesh();

		~SkeletalMesh();

		friend class MeshManager;
	};
}