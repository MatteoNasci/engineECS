#include "SimulationEvent.h"

void engineECS::SimulationEvent::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
{
}

void engineECS::SimulationEvent::onWake(physx::PxActor** actors, physx::PxU32 count)
{
}

void engineECS::SimulationEvent::onSleep(physx::PxActor** actors, physx::PxU32 count)
{
}

void engineECS::SimulationEvent::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{
}

void engineECS::SimulationEvent::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
{
}

void engineECS::SimulationEvent::onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)
{
}
