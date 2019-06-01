#pragma once
#include <queue>

#include "private.h"
#include "constants.h"
#include "World.h"

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
	private:
		Engine();
		~Engine();

		World worlds[MaxWorlds];
		WorldIndex currentWorldIndex;
		std::queue<WorldIndex> recycler;
	};
}

