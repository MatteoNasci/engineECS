#pragma once

#include <bitset>

#include "constants.h"

namespace engineECS
{
	struct EntityComponentMask final
	{
		std::bitset<MaxComponents> mask;
	};
}