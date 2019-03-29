#pragma once
#include <vector>
#include "private.h"
#include "TypeDefiner.h"
namespace engineECS {
	template<typename T>
	class Component
	{
	public:
		static TypeId GetTypeId()
		{
			static TypeId InternalTypeId = TypeDefiner::_TypeIdCounter++;
			return InternalTypeId;
		}

		static std::vector<T>& GetVector()
		{
			static std::vector<T> Vector;
			return Vector;
		}
	protected:
		Component(const Component&) = default;               // Copy constructor
		Component(Component&&) = default;                    // Move constructor
		Component& operator=(const Component&) = default;  // Copy assignment operator
		Component& operator=(Component&&) = default;       // Move assignment operator
		virtual ~Component() = default;                 // Destructor
		Component() = default;
	};
}