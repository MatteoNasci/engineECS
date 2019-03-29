#pragma once
#include <bitset>
namespace engineECS {
#define MAX_COMPONENTS 64
	struct EntityComponentMask
	{
		std::bitset<MAX_COMPONENTS> Mask;
	};
}