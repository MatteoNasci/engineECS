#pragma once

#include <functional>

#include "private.h"
#include "Entity.h"
#include "EntityComponentMask.h"

namespace engineECS
{
	struct System final
	{
	public:
		const std::function<void(const engineECS::Entity& inEntity, const float deltaTime)>& getCallback() const;
		const EntityComponentMask& getMask() const;

		template<typename... Components>
		void setMask()
		{
			[this](...) {}(this->mask.mask.set(Components::getTypeId(), true)...);
		}
		void setMask(const EntityComponentMask& mask);

		System();
		System(const std::function<void(const engineECS::Entity& inEntity, const float deltaTime)>& inCallback);
		System(const std::function<void(const engineECS::Entity& inEntity, const float deltaTime)>& inCallback, const EntityComponentMask& mask);
		System(const System&);
		System(System&&);
		System& operator=(const System&);
		System& operator=(System&&);
		~System();

		template<typename... Components>
		static System create(const std::function<void(const engineECS::Entity& inEntity, const float deltaTime)>& inCallback)
		{
			System system = System(inCallback);
			[&system](...) {}(system.mask.mask.set(Components::getTypeId(), true)...);
			return system;
		}
	private:
		std::function<void(const engineECS::Entity& inEntity, const float deltaTime)> callback;
		EntityComponentMask mask;
	};
}