#include "private.h"
#include "Entity.h"
#include "World.h"
#include "components.h"

int main(int argc, char **argv)
{
	engineECS::World world;

	engineECS::Entity enemy001 = world.CreateEntity();
	engineECS::Entity player = world.CreateEntity();
	engineECS::Entity enemy002 = world.CreateEntity();

	player.AddComponent<engineECS::RendererComponent>();
	enemy001.AddComponent<engineECS::RendererComponent>();
	enemy002.AddComponent<engineECS::RendererComponent>();


	world.DestroyEntity(player);

	world.ForEachAll([](const engineECS::Entity& InEntity) {
		std::printf("Hello from %d entity!\n", InEntity.Id);
	});

	std::getchar();
	return 0;
}