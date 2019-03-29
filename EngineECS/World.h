#pragma once

#include <functional>
#include <vector>
#include <queue>

#include "EntityComponentMask.h"
#include "Entity.h"
namespace engineECS {

	class World
	{
	public:
		Entity CreateEntity();

		void DestroyEntity(const Entity& InEntity);

		template<typename T>
		T& AddComponent(const Entity& InEntity) const
		{
			std::vector<T>& ComponentVector = T::GetVector();
			if (InEntity.Id >= ComponentVector.size())
			{
				ComponentVector.resize(InEntity.Id + 1);
			}
			ComponentVector[InEntity.Id] = {};
			size_t id = T::GetTypeId();
			Entities[InEntity.Id].Mask.set(id, true);
			return ComponentVector[InEntity.Id];
		}

		template<typename T>
		T& GetComponent(const Entity& InEntity) const
		{
			std::vector<T>& ComponentVector = T::GetVector();
			return ComponentVector[InEntity.Id];
		}

		void ForEachAll(const std::function<void(const Entity& InEntity)>& Callback) const
		{
			for (unsigned int i = 0; i < Entities.size(); i++)
			{
				if ((Entities[i].Mask.any()))
				{
					Callback(Entity(*this, i));
				}
			}
		}

		template<typename... Components>
		void ForEach(const std::function<void(const Entity& InEntity)>& Callback) const
		{
			std::bitset<MAX_COMPONENTS> Mask = 0;
			[&Mask](...) {}(Mask.set(Components::GetTypeId(), true)...);
			for (unsigned int i = 0; i < Entities.size(); i++)
			{
				if ((Entities[i].Mask & Mask) == Mask)
				{
					Callback(Entity(*this, i));
				}
			}
		}

	private:
		std::vector<EntityComponentMask> Entities;

		std::queue<unsigned int> Recycler;
	};
}

