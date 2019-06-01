#pragma once

#include <ostream>

#include "Component.h"
#include "constants.h"
#include "Vector3.h"

namespace engineECS
{
	struct TransformComponent : Component<TransformComponent, MaxEntities>
	{
	public:
		Vector3 position;
		Vector3 rotation;
		Vector3 scale;
	};

	static std::ostream& operator<<(std::ostream& lhs, const TransformComponent& rhs)
	{
		return lhs << "position: " << rhs.position << ", rotation: " << rhs.rotation << ", scale: " << rhs.scale;
	}
}