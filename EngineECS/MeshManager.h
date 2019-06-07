#pragma once

#include "Mesh.h"
#include "SkeletalMesh.h"

#include <array>
#include <vector>
#include <map>
#include <queue>
#include "Vector3.h"
#include "Matrix4.h"

namespace engineECS
{
	class MeshManager
	{
	public:
		static bool tryCreateSkeletalMesh(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::mat4>& bindPoses, const std::vector<std::array<int, 4>>& bones, const std::vector<std::array<float, 4>>& weights, const std::map<std::string, std::vector<std::vector<glm::mat4>>>& animations, int& meshIndex);
		static bool tryCreateSkeletalMesh(const std::vector<ffh::Vector3>& vertices, const std::vector<ffh::Vector3>& normals, const std::vector<ffh::Matrix4>& bindPoses, const std::vector<std::array<int, 4>>& bones, const std::vector<std::array<float, 4>>& weights, const std::map<std::string, std::vector<std::vector<ffh::Matrix4>>>& animations, int& meshIndex);
		static bool tryCreateMesh(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, int& meshIndex);
		static bool tryCreateMesh(const std::vector<ffh::Vector3>& vertices, const std::vector<ffh::Vector3>& normals, int& meshIndex);
		
		static void initialize();
		static bool isInitialized();
	private:
		static bool initialized;
		static std::queue<int> skeletalRecycler;
		static std::queue<int> meshRecycler;
	};
}

