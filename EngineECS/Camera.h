#pragma once
#include "Math.h"
namespace engineECS
{
	struct Camera
	{
	public:
		glm::mat4& getView();
		glm::mat4& getProjection();
		glm::mat4 view;
		glm::mat4 projection;

		Camera();
		~Camera();
	};
}

