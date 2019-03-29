#pragma once

#include "private.h"

namespace engineECS {
	class World;
	class Entity
	{
	private:
		Entity(const World& InWorld, unsigned int InId) : Id(InId), World(InWorld) {}

	public:
		const unsigned int Id;

		template<typename T>
		T& AddComponent()
		{
			return World.AddComponent<T>(*this);
		}

		template<typename T>
		T& GetComponent() const
		{
			return World.GetComponent<T>(*this);
		}

		const World& World;
		friend class World;
	};
}
