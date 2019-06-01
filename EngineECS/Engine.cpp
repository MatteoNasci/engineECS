#include "Engine.h"

void engineECS::Engine::run(const float deltaTime)
{
	for (currentWorldIndex = 0; currentWorldIndex < engineECS::MaxWorlds; ++currentWorldIndex)
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
	return engineECS::MaxWorlds - static_cast<int>(recycler.size());
}
int engineECS::Engine::getAvailableWorldsCount() const
{
	return engineECS::MaxWorlds - getActiveWorldsCount();
}
WorldIndex engineECS::Engine::createWorld(WorldCreation& outResult)
{
	if (getAvailableWorldsCount() <= 0)
	{
		outResult = WC_MAX_WORLD_LIMIT;
		return engineECS::InvalidWorldIndex;
	}

	WorldIndex index = recycler.front();
	recycler.pop();

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
	if (index >= engineECS::MaxWorlds)
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
engineECS::Engine::Engine()
{
	for (int i = 0; i < engineECS::MaxWorlds; ++i)
	{
		recycler.push(i);
		worlds[i].deinitialize();
	}
}
engineECS::Engine::~Engine()
{
	for (int i = 0; i < engineECS::MaxWorlds; ++i)
	{
		worlds[i].deinitialize();
	}
}