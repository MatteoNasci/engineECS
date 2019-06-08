#pragma once

#include "Mesh.h"
#include "SkeletalMesh.h"
#include "constants.h"

#include <array>
#include <vector>
#include <map>
#include <queue>
#include "Vector3.h"
#include "Matrix4.h"
#include "Vector2.h"
#include <PxPhysicsAPI.h>

namespace engineECS
{
	class MeshManager
	{
	public:
		bool tryCreateSkeletalMesh(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uvs, const std::vector<glm::mat4>& bindPoses, const std::vector<std::array<int, 4>>& bones, const std::vector<std::array<float, 4>>& weights, const std::map<std::string, std::vector<std::vector<glm::mat4>>>& animations, int& outMeshIndex);
		bool tryCreateSkeletalMesh(const std::vector<ffh::Vector3>& vertices, const std::vector<ffh::Vector3>& normals, const std::vector<ffh::Vector2>& uvs, const std::vector<ffh::Matrix4>& bindPoses, const std::vector<std::array<int, 4>>& bones, const std::vector<std::array<float, 4>>& weights, const std::map<std::string, std::vector<std::vector<ffh::Matrix4>>>& animations, int& outMeshIndex);
		bool tryCreateMesh(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& uvs, int& outMeshIndex);
		bool tryCreateMesh(const std::vector<ffh::Vector3>& vertices, const std::vector<ffh::Vector3>& normals, const std::vector<ffh::Vector2>& uvs, int& outMeshIndex);
		bool tryCreateMesh(const physx::PxShape* shape, int& outMeshIndex);

		MeshManager();

		engineECS::Mesh& getMesh(const int index);
		engineECS::SkeletalMesh& getSkeletalMesh(const int index);
		int getMaxMeshesCount() const;
		int getMaxSkeletalMeshesCount() const;
		int getMeshesCount() const;
		int getSkeletalMeshesCount() const;
		const engineECS::Mesh* getMeshes() const;
		const engineECS::SkeletalMesh* getSkeletalMeshes() const;
	private:
		bool tryAddMesh(const engineECS::Mesh& mesh, int& outIndex);
		bool tryAddSkeletalMesh(const engineECS::SkeletalMesh& mesh, int& outIndex);

		std::queue<int> skeletalRecycler;
		std::queue<int> meshRecycler;
		engineECS::Mesh meshes[engineECS::MaxUniqueMeshes];
		engineECS::SkeletalMesh skeletalMeshes[engineECS::MaxUniqueMeshes];
	};
}

