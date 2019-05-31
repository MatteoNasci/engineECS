#include "Engine.h"

engineECS::Engine& engineECS::Engine::getEngine()
{
	//TODO: test if this can be made inline
	static engineECS::Engine singletonEngine;
	return singletonEngine;
}
size_t engineECS::Engine::getActiveWorldsCount() const
{
	return getMaxWorldsCount() - recycler.size();
}
size_t engineECS::Engine::getMaxWorldsCount() const
{
	return engineECS::Engine::MaxWorlds;
}
size_t engineECS::Engine::getAvailableWorldsCount() const
{
	return getMaxWorldsCount() - getActiveWorldsCount();
}
WorldIndex engineECS::Engine::createWorld(WorldCreation& outResult)
{
	if (getAvailableWorldsCount() <= 0)
	{
		outResult = WC_MAX_WORLD_LIMIT;
		return getInvalidWorldIndex();
	}

	WorldIndex index = recycler.front();
	recycler.pop();

	worlds[index].initialize(index);

	outResult = WC_OK;
	return index;
}
engineECS::World* engineECS::Engine::getCurrentWorld()
{
	if (currentWorldIndex == engineECS::Engine::InvalidWorldIndex)
	{
		return nullptr;
	}
	return &worlds[currentWorldIndex];
}
WorldIndex engineECS::Engine::getCurrentWorldIndex() const
{
	return currentWorldIndex;
}
WorldIndex engineECS::Engine::getInvalidWorldIndex()
{
	return engineECS::Engine::InvalidWorldIndex;
}
engineECS::World* engineECS::Engine::getWorld(const WorldIndex index)
{
	return &worlds[index];
}
bool engineECS::Engine::tryDestroyWorld(const WorldIndex index)
{
	if (index >= getMaxWorldsCount())
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
	for (size_t i = 0; i < getMaxWorldsCount() - 1; i++)
	{
		recycler.push(i);
		worlds[i].deinitialize();
	}
}
engineECS::Engine::~Engine()
{
	for (size_t i = 0; i < getMaxWorldsCount() - 1; i++)
	{
		worlds[i].deinitialize();
	}
}