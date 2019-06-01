#pragma once

#include "Component.h"
#include "Vector3.h"
#include "constants.h"

namespace engineECS
{
	struct RotatorComponent : Component<RotatorComponent, MaxEntities>
	{
	public:
		Vector3 rotationSpeed;
	};
}