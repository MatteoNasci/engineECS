#pragma once
#include <queue>

#include "private.h"
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
		static constexpr size_t MaxWorlds = 10;
		static constexpr size_t InvalidWorldIndex = MaxWorlds + 1;

		Engine(const Engine&) = delete;
		void operator=(const Engine&) = delete;

		static Engine& getEngine();
		static WorldIndex getInvalidWorldIndex();

		WorldIndex createWorld(WorldCreation& outResult);
		bool tryDestroyWorld(const WorldIndex index);

		World* getWorld(const WorldIndex index);
		World* getCurrentWorld();
		WorldIndex getCurrentWorldIndex() const;
		size_t getActiveWorldsCount() const;
		size_t getMaxWorldsCount() const;
		size_t getAvailableWorldsCount() const;
	private:
		Engine();
		~Engine();

		World worlds[MaxWorlds];
		WorldIndex currentWorldIndex;
		std::queue<WorldIndex> recycler;
	};
}

