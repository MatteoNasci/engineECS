#pragma once
#include <queue>
#ifdef USE_ARRAY_WORLDS
#include <vector>
#endif

#include "private.h"
#include "constants.h"
#include "World.h"
#include "ShaderManager.h"
#include "MeshManager.h"
#include "PhysicsEngine.h"

namespace engineECS
{
	typedef enum WorldCreation
	{
		WC_OK = 0,
		WC_MAX_WORLD_LIMIT,
	} WorldCreation;

	class Engine
	{
	public:
		Engine(const Engine&) = delete;
		void operator=(const Engine&) = delete;

		static Engine& getEngine();

		WorldIndex createWorld(WorldCreation& outResult);
		bool tryDestroyWorld(const WorldIndex index);

		void run(const float deltaTime);

		World* getWorld(const WorldIndex index);
		World* getCurrentWorld();
		WorldIndex getCurrentWorldIndex() const;
		int getActiveWorldsCount() const;
		int getAvailableWorldsCount() const;

		engineECS::ShaderManager& getShaderManager();
		engineECS::MeshManager& getMeshManager();
		engineECS::PhysicsEngine& getPhysicsEngine();
	private:
		Engine();
		~Engine();

#ifdef USE_ARRAY_WORLDS
		World worlds[MaxWorlds];
#else
		std::vector<World> worlds;
#endif
		std::queue<WorldIndex> recycler;

		WorldIndex currentWorldIndex;

		engineECS::ShaderManager shaderManager;
		engineECS::MeshManager meshManager;
		engineECS::PhysicsEngine physicsEngine;
	};
}

