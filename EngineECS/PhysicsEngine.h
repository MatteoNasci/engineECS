#pragma once
#include "ErrorCallback.h"
#include "SimulationEvent.h"
#include "Allocator.h"
#include "Dispatcher.h"
#include <PxPhysicsAPI.h>
namespace engineECS
{
	class PhysicsEngine
	{
	public:
		PhysicsEngine();
		~PhysicsEngine();

		void update(const float deltatime);

		physx::PxFoundation* getFoundation();
		physx::PxPhysics* getPhysics();
		physx::PxScene* getScene();
	private:
		physx::PxFoundation* foundation;
		physx::PxPhysics* physics;
		physx::PxScene* scene;

		engineECS::Allocator allocator;
		engineECS::ErrorCallback errorCallback;
		engineECS::Dispatcher dispatcher;
		engineECS::SimulationEvent simulationEvent;

		physx::PxSceneDesc sceneDescription;
	};
}

