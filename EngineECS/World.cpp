#include "World.h"

#include "Engine.h"

engineECS::Entity engineECS::World::createEntity()
{
	size_t id = 0;
	if (entitiesRecycler.empty())
	{
		entities.push_back(EntityComponentMask());
		id = entities.size() - 1;
	}
	else
	{
		id = entitiesRecycler.front();
		entitiesRecycler.pop();
	}
	return Entity(id);
}
void engineECS::World::initialize(const WorldIndex inOwnIndex)
{
	ownIndex = inOwnIndex;
}
void engineECS::World::deinitialize()
{
	ownIndex = engineECS::Engine::getInvalidWorldIndex();
}
WorldIndex engineECS::World::getWorldIndex() const
{
	return ownIndex;
}
void engineECS::World::addSystem(const System& inSystem)
{
	//TODO
	//Systems.push_back(InSystem);
}
void engineECS::World::destroyEntity(const engineECS::Entity& inEntity)
{
	entitiesRecycler.push(inEntity.id);
	entities[inEntity.id].mask.reset();
}
bool engineECS::World::isActive() const
{
	return ownIndex != engineECS::Engine::getInvalidWorldIndex();
}
engineECS::World::World()
{
	deinitialize();
}
engineECS::World::~World()
{
	deinitialize();
}
size_t engineECS::World::getMaxSystems() const
{
	return engineECS::World::MaxSystems;
}
void engineECS::World::executeSystems()
{ 
	for (size_t i = 0; i < getMaxSystems(); ++i)
	{
		const auto& callback = systems[i].getCallback();
		if (callback)
		{
			//forEach()
		}
	}
}
void engineECS::World::forEachAll(const SYSTEM_FUNCTION& callback)
{
	for (size_t i = 0; i < entities.size(); ++i)
	{
		if ((entities[i].mask.any()))
		{
			callback(Entity(i));
		}
	}
}