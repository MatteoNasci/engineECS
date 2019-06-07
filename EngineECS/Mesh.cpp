#include "Mesh.h"

engineECS::Mesh::Mesh(const std::vector<glm::vec3>& inVertices, const std::vector<glm::vec3>& inNormals) : vertices(inVertices), normals(inNormals), uploaded(false), vao(), numVertices(), buffers()
{

}
engineECS::Mesh::Mesh(const std::vector<ffh::Vector3>& inVertices, const std::vector<ffh::Vector3>& inNormals) : uploaded(false), vao(), numVertices(), buffers()
{
	for (const ffh::Vector3& vertex : inVertices)
	{
		vertices.push_back(vector3FromFFHtoGLM(vertex));
	}
	for (const ffh::Vector3& normal : inNormals)
	{
		normals.push_back(vector3FromFFHtoGLM(normal));
	}
}
void engineECS::Mesh::upload()
{
	if (uploaded)
	{
		cleanup();
	}
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

	numVertices = static_cast<GLsizei>(vertices.size());

	uploaded = true;
}
void engineECS::Mesh::cleanup()
{
	//glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);

	glDeleteBuffers(static_cast<GLsizei>(buffers.size()), buffers.data());
	glDeleteVertexArrays(1, &vao);
	uploaded = false;
}
engineECS::Mesh::~Mesh()
{
	cleanup();
}
engineECS::Mesh::Mesh() : uploaded(false), vao(), numVertices(), buffers()
{

}
