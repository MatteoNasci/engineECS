#include "SkeletalMesh.h"
engineECS::SkeletalMesh::SkeletalMesh() : uploaded(false), vao(), numVertices(), buffers()
{

}
engineECS::SkeletalMesh::SkeletalMesh(const std::vector<glm::vec3>& inVertices, const std::vector<glm::vec3>& inNormals, const std::vector<glm::vec2>& inUvs, const std::vector<glm::mat4>& inBindPoses, const std::vector<std::array<int, 4>>& inBones, const std::vector<std::array<float, 4>>& inWeights, const std::map<std::string, std::vector<std::vector<glm::mat4>>>& inAnimations) : vertices(inVertices), normals(inNormals), uvs(inUvs), bindPoses(inBindPoses), bones(inBones), weights(inWeights), animations(inAnimations), uploaded(false), vao(), numVertices(), buffers()
{
}
engineECS::SkeletalMesh::SkeletalMesh(const std::vector<ffh::Vector3>& inVertices, const std::vector<ffh::Vector3>& inNormals, const std::vector<ffh::Vector2>& inUvs, const std::vector<ffh::Matrix4>& inBindPoses, const std::vector<std::array<int, 4>>& inBones, const std::vector<std::array<float, 4>>& inWeights, const std::map<std::string, std::vector<std::vector<ffh::Matrix4>>>& inAnimations) : bones(inBones), weights(inWeights), uploaded(false), vao(), numVertices(), buffers()
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
	for (const ffh::Matrix4& pose : inBindPoses)
	{
		bindPoses.push_back(matrix4FromFFHtoGLM(pose));
	}
	for (const auto& anim : inAnimations)
	{
		std::vector<std::vector<glm::mat4>> converted;
		for (const auto& list : anim.second)
		{
			std::vector<glm::mat4> matrices;
			for (const ffh::Matrix4& data : list)
			{
				matrices.push_back(matrix4FromFFHtoGLM(data));
			}
		}
		animations[anim.first] = converted;
	}
}
void engineECS::SkeletalMesh::upload()
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

	// normals
	glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float) * 2, uvs.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// bones
	glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
	glBufferData(GL_ARRAY_BUFFER, bones.size() * sizeof(int) * 4, bones.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(3, 4, GL_INT, 0, nullptr);

	// weights
	glBindBuffer(GL_ARRAY_BUFFER, buffers[4]);
	glBufferData(GL_ARRAY_BUFFER, weights.size() * sizeof(float) * 4, weights.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

	numVertices = static_cast<GLsizei>(vertices.size());

	uploaded = true;

	std::cout << "SkeletalMesh uploaded: " << this << std::endl;
}
void engineECS::SkeletalMesh::cleanup()
{
	if (uploaded)
	{
		//glDisableVertexAttribArray(0);
		//glDisableVertexAttribArray(1);
		//glDisableVertexAttribArray(2);
		//glDisableVertexAttribArray(3);

		glDeleteBuffers(static_cast<GLsizei>(buffers.size()), buffers.data());
		glDeleteVertexArrays(1, &vao);
		uploaded = false;

		std::cout << "SkeletalMesh cleanup: " << this << std::endl;
	}
}
engineECS::SkeletalMesh::~SkeletalMesh()
{
	cleanup();
}
