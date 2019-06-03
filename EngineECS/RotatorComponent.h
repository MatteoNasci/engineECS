#pragma once

#include "Component.h"
#include "Math.h"
#include "constants.h"

namespace engineECS
{
	struct RotatorComponent : Component<RotatorComponent, MaxEntities>
	{
	public:
		glm::vec3 rotationSpeed;
	};
}