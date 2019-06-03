
#include "private.h"
#include "Entity.h"
#include "World.h"
#include "components.h"
#include "Engine.h"
#include <iostream>
#include "OpenGLWrapper.h"
#include "constants.h"
#include "FpsManager.h"
#include "Mesh.h"
#include "Math.h"
#include "OpenGLShader.h"

int main(int argc, char **argv)
{
	{
		engineECS::WorldCreation creationResult;
		WorldIndex index = GET_ENGINE.createWorld(creationResult);

		if (creationResult != engineECS::WorldCreation::WC_OK)
		{
			//error
		}
		engineECS::World* world = GET_ENGINE.getWorld(index);

		engineECS::EntityCreation result;
		engineECS::Entity enemy001 = world->createEntity(result);
		engineECS::Entity player = world->createEntity(result);
		engineECS::Entity enemy002 = world->createEntity(result);

		world->tryAddComponent<engineECS::RendererComponent>(player);
		world->tryAddComponent<engineECS::RendererComponent>(enemy001);
		world->tryAddComponent<engineECS::RendererComponent>(enemy002);

		world->tryAddComponent<engineECS::TransformComponent>(player);
		world->tryAddComponent<engineECS::TransformComponent>(enemy001);

		world->tryAddComponent<engineECS::TransformComponent>(enemy002);
		world->getComponent<engineECS::TransformComponent>(enemy002).rotation = glm::vec3(0.2f, 10.0f, -55.3f);

		world->tryAddComponent<engineECS::RotatorComponent>(enemy002);
		world->getComponent<engineECS::RotatorComponent>(enemy002).rotationSpeed = glm::vec3(5, 1, 100);
		do
		{
			engineECS::Entity entity = world->createEntity(result);
			if (result == engineECS::EntityCreation::EC_OK)
			{
				world->tryAddComponent<engineECS::TransformComponent>(entity);
				world->tryAddComponent<engineECS::RotatorComponent>(entity);
				world->getComponent<engineECS::TransformComponent>(entity).rotation = glm::vec3(0.2f, 10.0f, -55.3f);
				world->getComponent<engineECS::RotatorComponent>(entity).rotationSpeed = glm::vec3(5, 1, 100);
			}
		} while (result == engineECS::EntityCreation::EC_OK);

		world->destroyEntity(player);

		world->tryAddSystem(engineECS::System::create<engineECS::TransformComponent>([](const engineECS::Entity& inEntity, const float deltaTime)
		{
			//std::cout << "Hello from " << inEntity.id << " entity with " << deltaTime << " delta time! (" << (deltaTime == 0.f ? -1 : 1.f / deltaTime) << ")" << std::endl;
		}));

		world->tryAddSystem(engineECS::System::create<engineECS::TransformComponent, engineECS::RotatorComponent>([](const engineECS::Entity& inEntity, const float deltaTime)
		{
			engineECS::TransformComponent& transform = inEntity.getComponent<engineECS::TransformComponent>();
			engineECS::RotatorComponent& rotator = inEntity.getComponent<engineECS::RotatorComponent>();
			transform.rotation += rotator.rotationSpeed * deltaTime;
		}));
	}

	engineECS::OpenGLWrapper wrapper = {};

	engineECS::FpsManager fpsManager = { &engineECS::OpenGLWrapper::getTime };
	fpsManager.setTargetFramePerSecond(-1.);

	engineECS::OpenGLShader defaultSkeletalShader(R"(
#version 410 core

layout(location=0) in vec3 vertex;
layout(location=1) in vec3 normal;
layout(location=2) in ivec4 bones;
layout(location=3) in vec4 weights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 bones_matrix[100];

out float lambert;
out vec3 bone_color;

vec3 bones_colors[6] = vec3[](
	vec3(0, 1, 0),
	vec3(0, 0, 1),
	vec3(1, 1, 0),
	vec3(1, 0, 1),
	vec3(0.2, 0.5, 1),
	vec3(0, 1, 1)
);

void main()
{
	bone_color = vec3(1, 1, 1);
	if (bones.x < 6)
	{
		bone_color = bones_colors[bones.x] * weights.x;
	}
	if (bones.y < 6)
	{
		bone_color += bones_colors[bones.y] * weights.y;
	}
	if (bones.z < 6)
	{
		bone_color += bones_colors[bones.z] * weights.z;
	}
	if (bones.w < 6)
	{
		bone_color += bones_colors[bones.w] * weights.w;
	}

	mat4 bone_matrix = bones_matrix[bones.x] * weights.x;
	bone_matrix += bones_matrix[bones.y] * weights.y;
	bone_matrix += bones_matrix[bones.z] * weights.z;
	bone_matrix += bones_matrix[bones.w] * weights.w;

	vec3 light_vector = vec3(0, 0, -1);
	vec4 world_vertex = model * bone_matrix * vec4(vertex, 1);
	vec4 world_normal = normalize(model * bone_matrix * vec4(normal, 0));
	lambert = clamp(dot(-light_vector, world_normal.xyz), 0, 1);
	
	gl_Position = projection * view * world_vertex;
}
)", R"(

#version 410 core

in float lambert;
out vec4 color;
in vec3 bone_color;

void main()
{
	color = vec4(bone_color, 1) * lambert;
}
)");
	engineECS::OpenGLShader defaultShader(R"(
#version 410 core

layout(location=0) in vec3 vertex;
layout(location=1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out float lambert;
out vec3 in_color;

void main()
{
	in_color = vec3(1, 0, 0);

	vec3 light_vector = vec3(0, 0, -1);
	vec4 world_vertex = model * vec4(vertex, 1);
	vec4 world_normal = normalize(model * vec4(normal, 0));
	lambert = clamp(dot(-light_vector, world_normal.xyz), 0, 1);
	
	gl_Position = projection * view * world_vertex;
}
)", R"(

#version 410 core

in float lambert;
out vec4 color;
in vec3 in_color;

void main()
{
	color = vec4(in_color, 1) * lambert;
}
)");

	while (!wrapper.shouldCloseWindow())
	{
		fpsManager.update();

		wrapper.prepareFrame();

		GET_ENGINE.run(fpsManager.getDeltaTime());

		wrapper.finalizeFrame();

		//TODO: DEBUG to remove
		if (fpsManager.getTotalFramesCount() % 200 == 0)
		{
			std::cout << "Current fps: " << fpsManager.getCurrentFramePerSecond() << ", saved: " << fpsManager.getSavedFramePerSecond() << ", max: " << fpsManager.getSavedMaxFramePerSecond() << ", min: " << fpsManager.getSavedMinFramePerSecond() << ", total fps: " << fpsManager.getTotalFramePerSecond() << " FPS." << std::endl;
		}
	}

	return 0;
}