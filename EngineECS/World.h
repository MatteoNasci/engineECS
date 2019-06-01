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
			if (inEntity.id >= T::getLength())
			{
				return false;
			}

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

		friend Engine;

	private:
		void forEach(const std::function<void(const engineECS::Entity& inEntity, const float deltaTime)>& callback, const EntityComponentMask& mask, const float deltaTime);

		EntityComponentMask entities[MaxEntities];
		std::queue<int> entitiesRecycler;

		System systems[MaxSystems];
		std::queue<int> systemsRecycler;
		
		World();
		~World();

		WorldIndex ownIndex;

		void initialize(const WorldIndex ownIndex);
		void deinitialize();

	};
}

