#pragma once

#include "Math.h"
#include <map>
#include <vector>
#include <array>

namespace engineECS
{
	struct Mesh
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;

		GLuint vao;
		std::array<GLuint, 2> buffers;
		bool uploaded;

		GLsizei numVertices;

		Mesh();

		~Mesh();

		void cleanup();

		void upload();
	};
}