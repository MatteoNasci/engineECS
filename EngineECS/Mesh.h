#pragma once

#include "Math.h"
#include <map>
#include <vector>
#include <array>
#include "Vector3.h"
#include "Vector2.h"
#include <PxPhysicsAPI.h>

namespace engineECS
{
	struct Mesh
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;

		GLuint vao;
		std::array<GLuint, 3> buffers;
		bool uploaded : 1;

		GLsizei numVertices;

		void cleanup();

		void upload();

	private:
		Mesh(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uvs);
		Mesh(const std::vector<ffh::Vector3>& vertices, const std::vector<ffh::Vector3>& normals, const std::vector<ffh::Vector2>& uvs);
		Mesh();
		Mesh(const physx::PxShape* shape);

		~Mesh();

		friend class MeshManager;
	};
}