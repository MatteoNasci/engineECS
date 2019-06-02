#pragma once
#include <vector>

#include "private.h"
#include "TypeDefiner.h"
#include "constants.h"

#ifdef USE_ARRAY_COMPONENTS
#define CALCULATE_LENGTH(desiredSize) ((desiredSize < 0 || desiredSize > MaxEntities) ? MaxEntities : desiredSize)
#endif

namespace engineECS
{
	template<typename T, int componentMaxCount>
	class Component
	{
	public:

		static int getLength()
		{
#ifdef USE_ARRAY_COMPONENTS
			static constexpr int length = CALCULATE_LENGTH(componentMaxCount);
			return length;
#else
			return static_cast<int>(getElements().size());
#endif
		}
		static TypeId getTypeId()
		{
			static const TypeId internalTypeId = TypeDefiner::_typeIdCounter++;
			return internalTypeId;
		}

		static T& getElement(const int index)
		{
			return getElements()[index];
		}

		static bool tryGetElement(const int index, T& outElement)
		{
#ifdef USE_ARRAY_COMPONENTS
			if (index >= getLength())
			{
				return false;
			}

			outElement = getElement(index);
			return true;
#else
			outElement = getElements().at(index);
			return true;
#endif
		}

#ifdef USE_ARRAY_COMPONENTS
		static T* getElements()
		{
			static T elements[CALCULATE_LENGTH(componentMaxCount)];
			return elements;
		}
#else
		static std::vector<T>& getElements()
		{
			static std::vector<T> elements;
			return elements;
		}
#endif
	protected:
		Component(const Component&) = default;              
		Component(Component&&) = default;                   
		Component& operator=(const Component&) = default; 
		Component& operator=(Component&&) = default;       
		~Component() = default;                 
		Component() = default;
	};
}