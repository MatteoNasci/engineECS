#pragma once
#include <PxPhysicsAPI.h>

namespace engineECS
{
	class Allocator : public physx::PxAllocatorCallback
	{
	public:

		// Inherited via PxAllocatorCallback
		void* allocate(size_t size, const char* typeName, const char* filename, int line) override;

		void deallocate(void* ptr) override;

	};
}

