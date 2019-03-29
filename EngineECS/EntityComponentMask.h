#pragma once
#include <bitset>
namespace engineECS {
#define MAX_COMPONENTS 256
	struct EntityComponentMask
	{
		std::bitset<MAX_COMPONENTS> Mask;
	};
}