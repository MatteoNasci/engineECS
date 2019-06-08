#pragma once
#include <PxPhysicsAPI.h>

namespace engineECS
{
	class ErrorCallback : public physx::PxErrorCallback
	{
	public:
		// Inherited via PxErrorCallback
		void reportError(physx::PxErrorCode::Enum code, const char * message, const char * file, int line) override;
	};
}

