#pragma once
#include <PxPhysicsAPI.h>

namespace engineECS
{
	class Dispatcher : public physx::PxCpuDispatcher
	{
	public:
		Dispatcher();
		~Dispatcher();
		// Inherited via PxCpuDispatcher
		void submitTask(physx::PxBaseTask& task) override;

		uint32_t getWorkerCount() const override;

	};
}

