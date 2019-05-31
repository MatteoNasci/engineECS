#pragma once

#include <bitset>

namespace engineECS
{
	struct EntityComponentMask final
	{
		static constexpr size_t MaxComponents = 256;
		std::bitset<MaxComponents> mask;
	};
}