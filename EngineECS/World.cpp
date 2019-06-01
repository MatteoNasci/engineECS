#include "World.h"

#include "Engine.h"

engineECS::Entity engineECS::World::createEntity(engineECS::EntityCreation& outResult)
{
	if (entitiesRecycler.empty())
	{
		outResult = engineECS::EntityCreation::EC_ENTITY_LIMIT;
		return Entity(engineECS::InvalidEntityIndex);
	}
	int id = entitiesRecycler.front();
	entitiesRecycler.pop();
	return Entity(id);
}
void engineECS::World::initialize(const WorldIndex inOwnIndex)
{
	ownIndex = inOwnIndex;
	std::memset(systems, 0, sizeof(engineECS::System) * engineECS::MaxSystems);
	for (int i = 0; i < engineECS::MaxSystems; ++i)
	{
		systemsRecycler.push(i);
	}
	std::memset(entities, 0, sizeof(engineECS::EntityComponentMask) * engineECS::MaxEntities);
	for (int i = 0; i < engineECS::MaxEntities; ++i)
	{
		entitiesRecycler.push(i);
	}
}
void engineECS::World::deinitialize()
{
	ownIndex = engineECS::InvalidWorldIndex;
}
WorldIndex engineECS::World::getWorldIndex() const
{
	return ownIndex;
}
bool engineECS::World::tryAddSystem(const System& inSystem)
{
	if (systemsRecycler.empty())
	{
		return false;
	}

	int index = systemsRecycler.front();
	systemsRecycler.pop();
	systems[index] = inSystem;
	return true;
}
bool engineECS::World::tryAddSystem(System&& inSystem)
{
	if (systemsRecycler.empty())
	{
		return false;
	}

	int index = systemsRecycler.front();
	systemsRecycler.pop();
	systems[index] = inSystem;
	return true;
}
void engineECS::World::destroyEntity(const engineECS::Entity& inEntity)
{
	if (inEntity.id == engineECS::InvalidEntityIndex)
	{
		return;
	}
	entitiesRecycler.push(inEntity.id);
	entities[inEntity.id].mask.reset();
}
bool engineECS::World::isActive() const
{
	return ownIndex != engineECS::InvalidWorldIndex;
}
engineECS::World::World()
{
	deinitialize();
}
engineECS::World::~World()
{
	deinitialize();
}
void engineECS::World::executeSystems(const float deltaTime)
{
	for (int i = 0; i < engineECS::MaxSystems; ++i)
	{
		forEach(systems[i], deltaTime);
	}
}
void engineECS::World::forEachAll(const std::function<void(const engineECS::Entity& inEntity, const float deltaTime)>& callback, const float deltaTime)
{
	for (int i = 0; i < engineECS::MaxEntities; ++i)
	{
		if ((entities[i].mask.any()))
		{
			callback(Entity(i), deltaTime);
		}
	}
}
void engineECS::World::forEach(const std::function<void(const engineECS::Entity& inEntity, const float deltaTime)>& callback, const engineECS::EntityComponentMask& mask, const float deltaTime)
{
	if (!callback || mask.mask.none())
	{
		return;
	}

	for (int i = 0; i < engineECS::MaxEntities; ++i)
	{
		if ((entities[i].mask & mask.mask) == mask.mask)
		{
			callback(Entity(i), deltaTime);
		}
	}
}
void engineECS::World::forEach(const engineECS::System& system, const float deltaTime)
{
	forEach(system.getCallback(), system.getMask(), deltaTime);
}
