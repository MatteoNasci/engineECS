#pragma once

#include <functional>

#include "private.h"
#include "Entity.h"

namespace engineECS
{
	class System final
	{
	public:
		const std::function<void(const engineECS::Entity& inEntity)>& getCallback() const;

		System(const std::function<void(const engineECS::Entity& inEntity)>& inCallback);
		~System();
	private:
		const std::function<void(const engineECS::Entity& inEntity)>& callback;

	};
}