#pragma once

#include "Component.h"
#include "Math.h"
#include "constants.h"

namespace engineECS
{
	struct TransformComponent : Component<TransformComponent, MaxEntities>
	{
	public:
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};

	static std::ostream& operator<<(std::ostream& lhs, const TransformComponent& rhs)
	{
		lhs << "Position: x|" << rhs.position.x << ", y|" << rhs.position.y << ", z|" << rhs.position.z << "." << std::endl;
		lhs << "Rotation: x|" << rhs.rotation.x << ", y|" << rhs.rotation.y << ", z|" << rhs.rotation.z << "." << std::endl;
		lhs << "Scale: x|" << rhs.scale.x << ", y|" << rhs.scale.y << ", z|" << rhs.scale.z << "." << std::endl;
		return lhs;
	}
}