#pragma once

#include <functional>
#include <vector>
#include <queue>
#include <bitset>

#include "private.h"
#include "EntityComponentMask.h"
#include "Entity.h"
#include "System.h"
#include "constants.h"
#include "Camera.h"
namespace engineECS 
{
	class Engine;

	typedef enum EntityCreation
	{
		EC_OK = 0,
		EC_ENTITY_LIMIT,
	}EntityCreation ;

	class World final
	{
	public:


		Entity createEntity(EntityCreation& outResult);

		void destroyEntity(const Entity& inEntity);

		void forEachAll(const std::function<void(const engineECS::Entity& inEntity, const float deltaTime)>& callback, const float deltaTime);

		void forEach(const System& system, const float deltaTime);


		template<typename... Components>
		void forEach(const std::function<void(const engineECS::Entity& inEntity, const float deltaTime)>& callback, const float deltaTime)
		{
			EntityComponentMask mask{};
			[&mask](...) {}(mask.mask.set(Components::getTypeId(), true)...);
			forEach(callback, mask, deltaTime);
		}

		template<typename T>
		bool tryAddComponent(const Entity& inEntity)
		{
#ifdef USE_ARRAY_COMPONENTS
			if (inEntity.id >= T::getLength())
			{
				return false;
			}
#else
			std::vector<T>& componentVector = T::getElements();
			if (inEntity.id >= componentVector.size())
			{
				componentVector.resize(inEntity.id + 1);
			}
#endif
			T::getElement(inEntity.id) = {};

			int id = T::getTypeId();
			entities[inEntity.id].mask.set(id, true);

			return true;
		}

		template<typename T>
		T& getComponent(const Entity& inEntity) const
		{
			return T::getElement(inEntity.id);
		}

		bool tryAddSystem(const System& inSystem);
		bool tryAddSystem(System&& inSystem);

		void executeSystems(const float deltaTime);
		WorldIndex getWorldIndex() const;
		bool isActive() const;
		engineECS::Camera& getMainCamera();

		friend Engine;

	private:
		void forEach(const std::function<void(const engineECS::Entity& inEntity, const float deltaTime)>& callback, const EntityComponentMask& mask, const float deltaTime);

#ifdef USE_ARRAY_ENTITIES
		EntityComponentMask entities[MaxEntities];
#else
		std::vector<EntityComponentMask> entities;
#endif
		std::queue<int> entitiesRecycler;

#ifdef USE_ARRAY_SYSTEMS
		System systems[MaxSystems];
#else
		std::vector<System> systems;
#endif
		std::queue<int> systemsRecycler;

		World();
#ifndef USE_ARRAY_WORLDS
	public:
#endif // !USE_ARRAY_WORLDS
		~World();
#ifndef USE_ARRAY_WORLDS
	private:
#endif // !USE_ARRAY_WORLDS

		WorldIndex ownIndex;
		engineECS::Camera mainCamera;

		void initialize(const WorldIndex ownIndex);
		void deinitialize();

	};
}

