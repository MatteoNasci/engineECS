#pragma once

#include "Math.h"
#include <map>
#include <vector>
#include <array>

namespace engineECS
{
	struct SkeletalMesh
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;

		std::vector<glm::mat4> bindPoses;

		std::vector<std::array<int, 4>> bones;
		std::vector<std::array<float, 4>> weights;

		std::map<std::string, std::vector<std::vector<glm::mat4>>> animations;

		GLuint vao;
		std::array<GLuint, 4> buffers;
		bool uploaded;

		GLsizei numVertices;

		SkeletalMesh();

		~SkeletalMesh();

		void cleanup();

		void upload();
	};
}