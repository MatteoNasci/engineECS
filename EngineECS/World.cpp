#include "World.h"

#include "Engine.h"

engineECS::Entity engineECS::World::createEntity(engineECS::EntityCreation& outResult)
{
#ifdef USE_ARRAY_ENTITIES
	if (entitiesRecycler.empty())
	{
		outResult = engineECS::EntityCreation::EC_ENTITY_LIMIT;
		return Entity(engineECS::InvalidEntityIndex);
	}
	outResult = engineECS::EntityCreation::EC_OK;
	int id = entitiesRecycler.front();
	entitiesRecycler.pop();
	return Entity(id);
#else
	outResult = engineECS::EntityCreation::EC_OK;
	int id = 0;
	if (entitiesRecycler.empty())
	{
		entities.push_back(EntityComponentMask());
		id = static_cast<int>(entities.size()) - 1;
	}
	else
	{
		id = entitiesRecycler.front();
		entitiesRecycler.pop();
	}
	return Entity(id);
#endif
}
void engineECS::World::initialize(const WorldIndex inOwnIndex)
{
	ownIndex = inOwnIndex;
#ifdef USE_ARRAY_SYSTEMS
	std::memset(systems, 0, sizeof(engineECS::System) * engineECS::MaxSystems);
	for (int i = 0; i < engineECS::MaxSystems; ++i)
	{
		systemsRecycler.push(i);
	}
#endif
#ifdef USE_ARRAY_ENTITIES
	std::memset(entities, 0, sizeof(engineECS::EntityComponentMask) * engineECS::MaxEntities);
	for (int i = 0; i < engineECS::MaxEntities; ++i)
	{
		entitiesRecycler.push(i);
	}
#endif
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
#ifdef USE_ARRAY_SYSTEMS
	if (systemsRecycler.empty())
	{
		return false;
	}

	int index = systemsRecycler.front();
	systemsRecycler.pop();
	systems[index] = inSystem;
	return true;
#else
	systems.push_back(inSystem);
	return true;
#endif
}
bool engineECS::World::tryAddSystem(System&& inSystem)
{
#ifdef USE_ARRAY_SYSTEMS
	if (systemsRecycler.empty())
	{
		return false;
	}

	int index = systemsRecycler.front();
	systemsRecycler.pop();
	systems[index] = inSystem;
	return true;
#else
	systems.push_back(inSystem);
	return true;
#endif
}
void engineECS::World::destroyEntity(const engineECS::Entity& inEntity)
{
#ifdef USE_ARRAY_ENTITIES
	if (inEntity.id == engineECS::InvalidEntityIndex)
	{
		return;
	}
	entitiesRecycler.push(inEntity.id);
	entities[inEntity.id].mask.reset();
#else
	entitiesRecycler.push(inEntity.id);
	entities[inEntity.id].mask.reset();
#endif
}
bool engineECS::World::isActive() const
{
	return ownIndex != engineECS::InvalidWorldIndex;
}
engineECS::World::World() : ownIndex(engineECS::InvalidWorldIndex)
{
	deinitialize();
}
engineECS::World::~World()
{
	deinitialize();
}
void engineECS::World::executeSystems(const float deltaTime)
{
#ifdef USE_ARRAY_SYSTEMS
	for (int i = 0; i < engineECS::MaxSystems; ++i)
	{
		forEach(systems[i], deltaTime);
	}
#else
	for (const engineECS::System& system : systems)
	{
		forEach(system, deltaTime);
	}
#endif
}
void engineECS::World::forEachAll(const std::function<void(const engineECS::Entity& inEntity, const float deltaTime)>& callback, const float deltaTime)
{
#ifdef USE_ARRAY_ENTITIES
	for (int i = 0; i < engineECS::MaxEntities; ++i)
	{
		if (entities[i].mask.any())
		{
			callback(Entity(i), deltaTime);
		}
	}
#else
	for (int i = 0; i < entities.size(); ++i)
	{
		if (entities[i].mask.any())
		{
			callback(Entity(i), deltaTime);
		}
	}
#endif
}
void engineECS::World::forEach(const std::function<void(const engineECS::Entity& inEntity, const float deltaTime)>& callback, const engineECS::EntityComponentMask& mask, const float deltaTime)
{
	if (!callback || mask.mask.none())
	{
		return;
	}

#ifdef USE_ARRAY_ENTITIES
	for (int i = 0; i < engineECS::MaxEntities; ++i)
	{
		if ((entities[i].mask & mask.mask) == mask.mask)
		{
			callback(Entity(i), deltaTime);
		}
	}
#else
	for (int i = 0; i < entities.size(); ++i)
	{
		if ((entities[i].mask & mask.mask) == mask.mask)
		{
			callback(Entity(i), deltaTime);
		}
	}
#endif
}
void engineECS::World::forEach(const engineECS::System& system, const float deltaTime)
{
	forEach(system.getCallback(), system.getMask(), deltaTime);
}
