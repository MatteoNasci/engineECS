#pragma once

#include <functional>
#include <vector>
#include <queue>

#include "private.h"
#include "EntityComponentMask.h"
#include "Entity.h"
#include "System.h"

namespace engineECS 
{
	class Engine;

	class World final
	{
	public:
		static constexpr size_t MaxSystems = 256;

		Entity createEntity();

		void destroyEntity(const Entity& inEntity);

		void forEachAll(const SYSTEM_FUNCTION& callback);

		template<typename... Components>
		void forEach(const SYSTEM_FUNCTION& callback)
		{
			std::bitset<engineECS::EntityComponentMask::MaxComponents> mask = 0;
			[&mask](...) {}(mask.set(Components::getTypeId(), true)...);
			for (size_t i = 0; i < entities.size(); ++i)
			{
				if ((entities[i].mask & mask) == mask)
				{
					callback(Entity(*this, i));
				}
			}
		}

		template<typename T>
		T& addComponent(const Entity& inEntity)
		{
			std::vector<T>& componentVector = T::getVector();
			if (inEntity.id >= componentVector.size())
			{
				componentVector.resize(inEntity.id + 1);
			}
			componentVector[inEntity.id] = {};
			size_t id = T::getTypeId();
			entities[inEntity.id].mask.set(id, true);
			return componentVector[inEntity.id];
		}

		template<typename T>
		T& getComponent(const Entity& inEntity) const
		{
			std::vector<T>& componentVector = T::getVector();
			return componentVector[inEntity.id];
		}

		void addSystem(const System& inSystem);

		void executeSystems();
		WorldIndex getWorldIndex() const;
		bool isActive() const;

		friend Engine;
	private:
		std::vector<EntityComponentMask> entities;
		std::queue<size_t> entitiesRecycler;

		SYSTEM_FUNCTION systems[MaxSystems];
		std::queue<size_t> systemsRecycler;

		World();
		~World();

		WorldIndex ownIndex;

		void initialize(const WorldIndex ownIndex);
		void deinitialize();

	};
}

