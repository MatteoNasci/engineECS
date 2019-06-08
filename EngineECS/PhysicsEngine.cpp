#include "PhysicsEngine.h"

#include <exception>

static physx::PxFilterFlags PhysxShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0, physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
	pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
	return physx::PxFilterFlag::eDEFAULT;
}


engineECS::PhysicsEngine::PhysicsEngine() : sceneDescription(physx::PxTolerancesScale()), allocator(), errorCallback(), dispatcher(), simulationEvent(), foundation(nullptr), physics(nullptr), scene(nullptr)
{
	foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocator, errorCallback);
	if (!foundation)
	{
		throw std::exception("unable to create foundation");
	}

	physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, physx::PxTolerancesScale());
	if (!physics)
	{
		throw std::exception("unable to create physics engine");
	}

	sceneDescription.gravity = physx::PxVec3(0, -9.81f, 0);
	sceneDescription.cpuDispatcher = &dispatcher;
	sceneDescription.simulationEventCallback = &simulationEvent;
	sceneDescription.filterShader = PhysxShader;
	scene = physics->createScene(sceneDescription);
	if (!scene)
	{
		throw std::exception("unable to create scene");
	}
}
physx::PxFoundation* engineECS::PhysicsEngine::getFoundation()
{
	return foundation;
}
physx::PxPhysics* engineECS::PhysicsEngine::getPhysics()
{
	return physics;
}
physx::PxScene* engineECS::PhysicsEngine::getScene()
{
	return scene;
}
void engineECS::PhysicsEngine::update(const float deltatime)
{
	scene->lockWrite();
	scene->simulate(deltatime);
	scene->fetchResults(true);
	scene->unlockWrite();
}
engineECS::PhysicsEngine::~PhysicsEngine()
{
}
