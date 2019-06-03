#include "Engine.h"

void engineECS::Engine::run(const float deltaTime)
{
#ifdef USE_ARRAY_WORLDS
	for (currentWorldIndex = 0; currentWorldIndex < engineECS::MaxWorlds; ++currentWorldIndex)
#else
	for (currentWorldIndex = 0; currentWorldIndex < worlds.size(); ++currentWorldIndex)
#endif
	{
		World& world = worlds[currentWorldIndex];
		if (world.isActive())
		{
			world.executeSystems(deltaTime);
		}
	}
	currentWorldIndex = engineECS::InvalidWorldIndex;
}
engineECS::Engine& engineECS::Engine::getEngine()
{
	static engineECS::Engine singletonEngine;
	return singletonEngine;
}
int engineECS::Engine::getActiveWorldsCount() const
{
#ifdef USE_ARRAY_WORLDS
	return engineECS::MaxWorlds - static_cast<int>(recycler.size());
#else
	return static_cast<int>(worlds.size() - recycler.size());
#endif
}
int engineECS::Engine::getAvailableWorldsCount() const
{
#ifdef USE_ARRAY_WORLDS
	return engineECS::MaxWorlds - getActiveWorldsCount();
#else
	return INT32_MAX;
#endif
}
WorldIndex engineECS::Engine::createWorld(WorldCreation& outResult)
{
	if (getAvailableWorldsCount() <= 0)
	{
		outResult = engineECS::WorldCreation::WC_MAX_WORLD_LIMIT;
		return engineECS::InvalidWorldIndex;
	}

#ifdef USE_ARRAY_WORLDS
	WorldIndex index = recycler.front();
	recycler.pop();
#else
	WorldIndex index = engineECS::InvalidWorldIndex;
	if (recycler.empty())
	{
		index = static_cast<int>(worlds.size());
		worlds.push_back(engineECS::World());
	}
	else
	{
		index = recycler.front();
		recycler.pop();
	}
#endif

	worlds[index].initialize(index);

	outResult = WC_OK;
	return index;
}
engineECS::World* engineECS::Engine::getCurrentWorld()
{
	if (currentWorldIndex == engineECS::InvalidWorldIndex)
	{
		return nullptr;
	}

	World& world = worlds[currentWorldIndex];
	if (!world.isActive())
	{
		return nullptr;
	}

	return &world;
}
WorldIndex engineECS::Engine::getCurrentWorldIndex() const
{
	return currentWorldIndex;
}
engineECS::World* engineECS::Engine::getWorld(const WorldIndex index)
{
	return &worlds[index];
}
bool engineECS::Engine::tryDestroyWorld(const WorldIndex index)
{
#ifdef USE_ARRAY_WORLDS
	if (index >= engineECS::MaxWorlds || index < 0)
#else
	if (index >= worlds.size() || index < 0)
#endif
	{
		return false;
	}

	engineECS::World& worldToDestroy = worlds[index];
	if (!worldToDestroy.isActive())
	{
		return false;
	}

	worldToDestroy.deinitialize();
	recycler.push(index);
	return true;
}
engineECS::Engine::Engine() : currentWorldIndex(engineECS::InvalidWorldIndex)
{
#ifdef USE_ARRAY_WORLDS
	for (int i = 0; i < engineECS::MaxWorlds; ++i)
	{
		recycler.push(i);
		worlds[i].deinitialize();
	}
#endif
}
engineECS::Engine::~Engine()
{
	for (int i = 0; i < engineECS::MaxWorlds; ++i)
	{
		worlds[i].deinitialize();
	}
}