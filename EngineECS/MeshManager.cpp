#include "MeshManager.h"
#include "constants.h"
bool engineECS::MeshManager::initialized = false;
std::queue<int> engineECS::MeshManager::meshRecycler;
std::queue<int> engineECS::MeshManager::skeletalRecycler;

bool engineECS::MeshManager::isInitialized()
{
	return engineECS::MeshManager::initialized;
}
void engineECS::MeshManager::initialize()
{
	if (!engineECS::MeshManager::initialized)
	{
		engineECS::MeshManager::initialized = !engineECS::MeshManager::initialized;
		while (engineECS::MeshManager::meshRecycler.size() > 0)
		{
			engineECS::MeshManager::meshRecycler.pop();
		}
		while (engineECS::MeshManager::skeletalRecycler.size() > 0)
		{
			engineECS::MeshManager::skeletalRecycler.pop();
		}

		for (register int i = 0; i < engineECS::MaxUniqueMeshes; ++i)
		{
			engineECS::MeshManager::skeletalRecycler.push(i);
			engineECS::MeshManager::meshRecycler.push(i);
		}
	}
}
bool engineECS::MeshManager::tryCreateSkeletalMesh(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::mat4>& bindPoses, const std::vector<std::array<int, 4>>& bones, const std::vector<std::array<float, 4>>& weights, const std::map<std::string, std::vector<std::vector<glm::mat4>>>& animations, int& meshIndex)
{
	return false;
}
bool engineECS::MeshManager::tryCreateSkeletalMesh(const std::vector<ffh::Vector3>& vertices, const std::vector<ffh::Vector3>& normals, const std::vector<ffh::Matrix4>& bindPoses, const std::vector<std::array<int, 4>>& bones, const std::vector<std::array<float, 4>>& weights, const std::map<std::string, std::vector<std::vector<ffh::Matrix4>>>& animations, int& meshIndex)
{
	return false;
}
bool engineECS::MeshManager::tryCreateMesh(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, int& meshIndex)
{
	return false;
}
bool engineECS::MeshManager::tryCreateMesh(const std::vector<ffh::Vector3>& vertices, const std::vector<ffh::Vector3>& normals, int& meshIndex)
{
	return false;
}
