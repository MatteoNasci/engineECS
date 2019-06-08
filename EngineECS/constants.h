#pragma once

#include "private.h"

namespace engineECS
{
	static constexpr int MaxWorlds = 5;
	static constexpr int InvalidWorldIndex = -1;
	static constexpr int MaxComponents = 256;
	static constexpr int MaxSystems = 256;
	static constexpr int MaxEntities = 1; //TODO: target 20000
	static constexpr int InvalidEntityIndex = -1;
	static constexpr int MaxUniqueMeshes = 8192;//8192;//65536;
	static constexpr int MaxUniquePrograms = 8192; //8192;//65536;
}