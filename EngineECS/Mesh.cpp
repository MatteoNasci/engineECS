#include "Mesh.h"
#include <stdint.h>

engineECS::Mesh::Mesh(const std::vector<glm::vec3>& inVertices, const std::vector<glm::vec3>& inNormals, const std::vector<glm::vec2>& inUvs) : vertices(inVertices), normals(inNormals), uvs(inUvs), uploaded(false), vao(), numVertices(), buffers()
{

}
engineECS::Mesh::Mesh(const std::vector<ffh::Vector3>& inVertices, const std::vector<ffh::Vector3>& inNormals, const std::vector<ffh::Vector2>& inUvs) : uploaded(false), vao(), numVertices(), buffers()
{
	for (const ffh::Vector3& vertex : inVertices)
	{
		vertices.push_back(vector3FromFFHtoGLM(vertex));
	}
	for (const ffh::Vector3& normal : inNormals)
	{
		normals.push_back(vector3FromFFHtoGLM(normal));
	}
	for (const ffh::Vector2& uv : inUvs)
	{
		uvs.push_back(vector2FromFFHtoGLM(uv));
	}
}
void engineECS::Mesh::upload()
{
	cleanup();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(static_cast<GLsizei>(buffers.size()), buffers.data());

	// vertices
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * 3, vertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// normals
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float) * 3, normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	//uvs
	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float) * 2, uvs.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	numVertices = static_cast<GLsizei>(vertices.size());

	uploaded = true;

	std::cout << "Mesh uploaded: " << this << std::endl;
}
engineECS::Mesh::Mesh(const physx::PxShape* shape)
{
	if (!shape)
	{
		throw std::exception("unable to create shape");
	}

	physx::PxTriangleMeshGeometry geometry;
	if (!shape->getTriangleMeshGeometry(geometry))
	{
		throw std::exception("unable to get triangle mesh geometry");
	}

	const physx::PxVec3* geometryVertices = geometry.triangleMesh->getVertices();
	if (!geometryVertices)
	{
		throw std::exception("unable to get vertices");
	}

	const void* genericTriangles = geometry.triangleMesh->getTriangles();
	if (!genericTriangles)
	{
		throw std::exception("unable to get triangles");
	}

	const physx::PxTriangleMeshFlags flags = geometry.triangleMesh->getTriangleMeshFlags();
	if (flags.isSet(physx::PxTriangleMeshFlag::Enum::e16_BIT_INDICES))
	{
		const int16_t* triangles = (int16_t*)(genericTriangles);
		for (size_t i = 0; i < geometry.triangleMesh->getNbTriangles(); i += 3)
		{
			const glm::vec3 vertex1 = vector3FromPxtoGLM(geometryVertices[triangles[i]]);
			const glm::vec3 vertex2 = vector3FromPxtoGLM(geometryVertices[triangles[i + 1]]);
			const glm::vec3 vertex3 = vector3FromPxtoGLM(geometryVertices[triangles[i + 2]]);

			vertices.push_back(vertex1);
			vertices.push_back(vertex2);
			vertices.push_back(vertex3);

			normals.push_back(glm::normalize(glm::cross(vertex2 - vertex1, vertex3 - vertex1)));
			normals.push_back(glm::normalize(glm::cross(vertex3 - vertex2, vertex1 - vertex2)));
			normals.push_back(glm::normalize(glm::cross(vertex1 - vertex3, vertex2 - vertex3)));
		}
	}
	else
	{
		const int32_t* triangles = (int32_t*)(genericTriangles);
		for (size_t i = 0; i < geometry.triangleMesh->getNbTriangles(); i += 3)
		{
			const glm::vec3 vertex1 = vector3FromPxtoGLM(geometryVertices[triangles[i]]);
			const glm::vec3 vertex2 = vector3FromPxtoGLM(geometryVertices[triangles[i + 1]]);
			const glm::vec3 vertex3 = vector3FromPxtoGLM(geometryVertices[triangles[i + 2]]);

			vertices.push_back(vertex1);
			vertices.push_back(vertex2);
			vertices.push_back(vertex3);

			normals.push_back(glm::normalize(glm::cross(vertex2 - vertex1, vertex3 - vertex1)));
			normals.push_back(glm::normalize(glm::cross(vertex3 - vertex2, vertex1 - vertex2)));
			normals.push_back(glm::normalize(glm::cross(vertex1 - vertex3, vertex2 - vertex3)));
		}
	}
}
void engineECS::Mesh::cleanup()
{
	if (uploaded)
	{
		//glDisableVertexAttribArray(0);
		//glDisableVertexAttribArray(1);

		glDeleteBuffers(static_cast<GLsizei>(buffers.size()), buffers.data());
		glDeleteVertexArrays(1, &vao);
		uploaded = false;

		std::cout << "Mesh cleanup: " << this << std::endl;
	}
}
engineECS::Mesh::~Mesh()
{
	cleanup();
}
engineECS::Mesh::Mesh() : uploaded(false), vao(), numVertices(), buffers()
{

}
