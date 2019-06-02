
#include "private.h"
#include "Entity.h"
#include "World.h"
#include "components.h"
#include "Engine.h"
#include <iostream>
#include "OpenGLWrapper.h"
#include "constants.h"

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

		std::cout << sizeof(std::function<void(const engineECS::Entity& inEntity, const float deltaTime)>) << std::endl;

		engineECS::EntityCreation result;
		engineECS::Entity enemy001 = world->createEntity(result);
		engineECS::Entity player = world->createEntity(result);
		engineECS::Entity enemy002 = world->createEntity(result);

		player.tryAddComponent<engineECS::RendererComponent>();
		enemy001.tryAddComponent<engineECS::RendererComponent>();
		enemy002.tryAddComponent<engineECS::RendererComponent>();

		player.tryAddComponent<engineECS::TransformComponent>();
		enemy001.tryAddComponent<engineECS::TransformComponent>();

		enemy002.tryAddComponent<engineECS::TransformComponent>();
		enemy002.getComponent<engineECS::TransformComponent>().rotation = engineECS::Vector3(0.2f, 10.0f, -55.3f);

		enemy002.tryAddComponent<engineECS::RotatorComponent>();
		enemy002.getComponent<engineECS::RotatorComponent>().rotationSpeed = engineECS::Vector3(5, 1, 100);

		for (size_t i = 0; i < engineECS::MaxEntities - 5; i++)
		{
			engineECS::EntityCreation result;
			engineECS::Entity entity = world->createEntity(result);
			entity.tryAddComponent<engineECS::TransformComponent>();
			entity.tryAddComponent<engineECS::RotatorComponent>();
			entity.getComponent<engineECS::TransformComponent>().rotation = engineECS::Vector3(0.2f, 10.0f, -55.3f);
			entity.getComponent<engineECS::RotatorComponent>().rotationSpeed = engineECS::Vector3(5, 1, 100);
		}

		world->destroyEntity(player);

		world->tryAddSystem(engineECS::System::create<engineECS::TransformComponent>([](const engineECS::Entity& inEntity, const float deltaTime)
		{
			//std::cout << "Hello from " << inEntity.id << " entity with " << deltaTime << " delta time! (" << (deltaTime == 0.f ? -1 : 1.f / deltaTime) << ")" << std::endl;
		}));

		world->tryAddSystem(engineECS::System::create<engineECS::TransformComponent, engineECS::RotatorComponent>([](const engineECS::Entity& inEntity, const float deltaTime)
		{
			engineECS::TransformComponent& transform = inEntity.getComponent<engineECS::TransformComponent>();
			engineECS::RotatorComponent& rotator = inEntity.getComponent<engineECS::RotatorComponent>();
			transform.rotation += engineECS::Vector3(rotator.rotationSpeed) * deltaTime;
		}));
	}

	engineECS::OpenGLWrapper wrapper = {};

	wrapper.setTargetFramePerSecond(-1.);

	while (!wrapper.shouldCloseWindow())
	{
		wrapper.prepareFrame();

		GET_ENGINE.run(wrapper.getDeltaTime());

		wrapper.finalizeFrame();

		if (wrapper.getTotalFramesCount() % 200 == 0)
		{
			std::cout << "frame end" << " with " << wrapper.getDeltaTime() << " delta time! (Current fps: " << wrapper.getCurrentFramePerSecond() << ", saved: " << wrapper.getSavedFramePerSecond() << ", max: " << wrapper.getSavedMaxFramePerSecond() << ", min: " << wrapper.getSavedMinFramePerSecond() << ", total fps: " << wrapper.getTotalFramePerSecond() << " FPS)." << std::endl;
		}
	}

	return 0;
}