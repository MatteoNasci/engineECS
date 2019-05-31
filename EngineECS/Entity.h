#pragma once

#include "private.h"

namespace engineECS 
{
	class World;

	struct Entity final
	{
	public:
		const TypeId id;

		template<typename T>
		T& addComponent()
		{
			return GET_CURRENT_WORLD->addComponent<T>(*this);
		}

		template<typename T>
		T& getComponent() const
		{
			return GET_CURRENT_WORLD->getComponent<T>(*this);
		}


		friend World;
	private:
		Entity(const TypeId inId) : id(inId) {}

	};
}

