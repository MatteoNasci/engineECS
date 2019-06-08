#include "MeshManager.h"

engineECS::Mesh& engineECS::MeshManager::getMesh(const int index)
{
	return meshes[index];
}
engineECS::SkeletalMesh& engineECS::MeshManager::getSkeletalMesh(const int index)
{
	return skeletalMeshes[index];
}
int engineECS::MeshManager::getMaxMeshesCount() const
{
	return engineECS::MaxUniqueMeshes;
}
int engineECS::MeshManager::getMaxSkeletalMeshesCount() const
{
	return engineECS::MaxUniqueMeshes;
}
int engineECS::MeshManager::getMeshesCount() const
{
	return getMaxMeshesCount() - static_cast<int>(meshRecycler.size());
}
int engineECS::MeshManager::getSkeletalMeshesCount() const
{
	return getMaxSkeletalMeshesCount() - static_cast<int>(skeletalRecycler.size());
}
const engineECS::Mesh* engineECS::MeshManager::getMeshes() const
{
	return meshes;
}
const engineECS::SkeletalMesh* engineECS::MeshManager::getSkeletalMeshes() const
{
	return skeletalMeshes;
}
engineECS::MeshManager::MeshManager()
{
	for (register int i = 0; i < engineECS::MaxUniqueMeshes; ++i)
	{
		engineECS::MeshManager::skeletalRecycler.push(i);
		engineECS::MeshManager::meshRecycler.push(i);
	}
}
bool engineECS::MeshManager::tryAddMesh(const engineECS::Mesh& mesh, int& outIndex)
{
	if (engineECS::MeshManager::meshRecycler.size() == 0)
	{
		outIndex = -1;
		return false;
	}

	outIndex = engineECS::MeshManager::meshRecycler.front();
	engineECS::MeshManager::meshRecycler.pop();

	meshes[outIndex] = mesh;
	meshes[outIndex].upload();

	return true;
}
bool engineECS::MeshManager::tryCreateMesh(const physx::PxShape* shape, int& outMeshIndex)
{
	if (!shape)
	{
		return false;
	}
	return tryAddMesh(engineECS::Mesh(shape), outMeshIndex);
}
bool engineECS::MeshManager::tryAddSkeletalMesh(const engineECS::SkeletalMesh& mesh, int& outIndex)
{
	if (engineECS::MeshManager::skeletalRecycler.size() == 0)
	{
		outIndex = -1;
		return false;
	}

	outIndex = engineECS::MeshManager::skeletalRecycler.front();
	engineECS::MeshManager::skeletalRecycler.pop();

	skeletalMeshes[outIndex] = mesh;
	skeletalMeshes[outIndex].upload();

	return true;
}
bool engineECS::MeshManager::tryCreateSkeletalMesh(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uvs, const std::vector<glm::mat4>& bindPoses, const std::vector<std::array<int, 4>>& bones, const std::vector<std::array<float, 4>>& weights, const std::map<std::string, std::vector<std::vector<glm::mat4>>>& animations, int& outMeshIndex)
{
	return tryAddSkeletalMesh(engineECS::SkeletalMesh(vertices, normals, uvs, bindPoses, bones, weights, animations), outMeshIndex);
}
bool engineECS::MeshManager::tryCreateSkeletalMesh(const std::vector<ffh::Vector3>& vertices, const std::vector<ffh::Vector3>& normals, const std::vector<ffh::Vector2>& uvs, const std::vector<ffh::Matrix4>& bindPoses, const std::vector<std::array<int, 4>>& bones, const std::vector<std::array<float, 4>>& weights, const std::map<std::string, std::vector<std::vector<ffh::Matrix4>>>& animations, int& outMeshIndex)
{
	return tryAddSkeletalMesh(engineECS::SkeletalMesh(vertices, normals, uvs, bindPoses, bones, weights, animations), outMeshIndex);
}
bool engineECS::MeshManager::tryCreateMesh(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uvs, int& outMeshIndex)
{
	return tryAddMesh(engineECS::Mesh(vertices, normals, uvs), outMeshIndex);
}
bool engineECS::MeshManager::tryCreateMesh(const std::vector<ffh::Vector3>& vertices, const std::vector<ffh::Vector3>& normals, const std::vector<ffh::Vector2>& uvs, int& outMeshIndex)
{
	return tryAddMesh(engineECS::Mesh(vertices, normals, uvs), outMeshIndex);
}
