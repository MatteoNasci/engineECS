#include "ErrorCallback.h"
#include <iostream>

// Inherited via PxErrorCallback
void engineECS::ErrorCallback::reportError(physx::PxErrorCode::Enum code, const char * message, const char * file, int line)
{
	std::cerr << message << std::endl;
}