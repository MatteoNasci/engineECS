#include "Mesh.h"

engineECS::Mesh::Mesh() : uploaded(false), vao(), numVertices(), buffers()
{
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

	// bones
	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, bones.size() * sizeof(int) * 4, bones.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribIPointer(2, 4, GL_INT, 0, nullptr);

	// weights
	glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
	glBufferData(GL_ARRAY_BUFFER, weights.size() * sizeof(float) * 4, weights.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

	numVertices = static_cast<GLsizei>(vertices.size());

	uploaded = true;
}
void engineECS::Mesh::cleanup()
{
	glDeleteBuffers(static_cast<GLsizei>(buffers.size()), buffers.data());
	glDeleteVertexArrays(1, &vao);
	uploaded = false;
}
engineECS::Mesh::~Mesh()
{
	cleanup();
}
