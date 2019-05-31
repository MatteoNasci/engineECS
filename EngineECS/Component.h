#pragma once
#include <vector>

#include "private.h"
#include "TypeDefiner.h"

namespace engineECS 
{
	template<typename T>
	class Component
	{
	public:
		static TypeId getTypeId()
		{
			static TypeId internalTypeId = TypeDefiner::_typeIdCounter++;
			return internalTypeId;
		}

		static std::vector<T>& getVector()
		{
			static std::vector<T> vector;
			return vector;
		}
	protected:
		Component(const Component&) = default;               // Copy constructor
		Component(Component&&) = default;                    // Move constructor
		Component& operator=(const Component&) = default;  // Copy assignment operator
		Component& operator=(Component&&) = default;       // Move assignment operator
		~Component() = default;                 // Destructor
		Component() = default;
	};
}