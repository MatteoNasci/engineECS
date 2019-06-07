#pragma once

#include "Math.h"
#include <map>
#include <vector>
#include <array>
#include "Vector3.h"

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

		void cleanup();

		void upload();

	private:
		Mesh(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals);
		Mesh(const std::vector<ffh::Vector3>& vertices, const std::vector<ffh::Vector3>& normals);
		Mesh();

		~Mesh();

		friend class MeshManager;
	};
}