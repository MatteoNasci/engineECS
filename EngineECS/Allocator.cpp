#include "Allocator.h"

void* engineECS::Allocator::allocate(size_t size, const char * typeName, const char * filename, int line)
{
	return malloc(size);
}

void engineECS::Allocator::deallocate(void * ptr)
{
	free(ptr);
}
