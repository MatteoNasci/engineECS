#pragma once
#include <vector>

#include "private.h"
#include "TypeDefiner.h"
#include "constants.h"

#define CALCULATE_LENGTH(desiredSize) ((desiredSize < 0 || desiredSize > MaxEntities) ? MaxEntities : desiredSize)

namespace engineECS
{
	template<typename T, int componentMaxCount>
	class Component
	{
	public:

		static int getLength()
		{
			static constexpr int length = CALCULATE_LENGTH(componentMaxCount);
			return length;
		}
		static TypeId getTypeId()
		{
			static TypeId internalTypeId = TypeDefiner::_typeIdCounter++;
			return internalTypeId;
		}

		static T& getElement(const int index)
		{
			return getElements()[index];
		}

		static bool tryGetElement(const int index, T& outElement)
		{
			if (index >= getLength())
			{
				return false;
			}

			outElement = getElement(index);
			return true;
		}

		static T* getElements()
		{
			static T elements[CALCULATE_LENGTH(componentMaxCount)];
			return elements;
		}
	protected:
		Component(const Component&) = default;              
		Component(Component&&) = default;                   
		Component& operator=(const Component&) = default; 
		Component& operator=(Component&&) = default;       
		~Component() = default;                 
		Component() = default;
	};
}