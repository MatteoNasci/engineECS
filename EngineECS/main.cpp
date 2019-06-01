#include "private.h"
#include "Entity.h"
#include "World.h"
#include "components.h"
#include "Engine.h"
#include "lua.hpp"
#include <iostream>
/*
static int lua_test(lua_State* L)
{
	std::cout << lua_gettop(L) << ", function stack top type: " << luaL_typename(L, -1) << std::endl;
	return luaL_error(L, "Brutto errore!");
	lua_pushstring(L, "random string");
	return 1;
}*/

int main(int argc, char **argv)
{
	/*
	//created lua Virtual Machine (global table). � tecnicamente una stack machine
	lua_State* L = luaL_newstate();
	//Cariva nella global table altre sub tables come string, coroutines, tables etc.. . Se non ci serve la standard library di lua non serve questa chiamata (si risparmia memoria)
	luaL_openlibs(L);

	luaL_loadfile(L, "test.lua");
	std::cout << lua_gettop(L) << ", stack top type: " << luaL_typename(L, -1) << std::endl;

	//quando funzione viene chiamata viene creato nuovo stack per gli argomenti per comodit�
	if (lua_pcall(L, 0, 1, 0) != 0) {
		std::cout << lua_tostring(L, -1) << std::endl;
	}

	std::cout << lua_gettop(L) << ", stack top type: " << luaL_typename(L, -1) << std::endl;

	lua_pop(L, 1);

	//set c++ function into global table with given key
	lua_pushglobaltable(L);
	lua_pushstring(L, "test");
	lua_pushcfunction(L, lua_test);
	lua_settable(L, -3);

	luaL_loadfile(L, "test.lua");
	std::cout << lua_gettop(L) << ", stack top type: " << luaL_typename(L, -1) << std::endl;

	if (lua_pcall(L, 0, 1, 0) != 0) {
		std::cout << lua_tostring(L, -1) << std::endl;
	}

	std::cout << lua_gettop(L) << ", stack top type: " << luaL_typename(L, -1) << std::endl;

	lua_pop(L, 2);

	//pusho table (in questo caso quella globale)
	lua_pushglobaltable(L);
	//pusho la chiave che voglio
	lua_pushstring(L, "_VERSION");
	//prendo la table a -2 nello stack, lui poi prender� (poppa) la chiave a -1.Risultato viene messo in cima allo stack
	lua_gettable(L, -2);
	std::cout << (lua_tostring(L, -1)) << ", stack top count: " << lua_gettop(L) << ", stack top type: " << luaL_typename(L, -1) << std::endl;
	//libero lo stack per tot roba
	lua_pop(L, 1);

	lua_pushstring(L, "matteo_key");
	lua_pushstring(L, "matteo_value");
	lua_settable(L, -3);//settata chiave e relativo valore nella table global, poppate chiave e valore

	lua_pop(L, 1);//poppata table

	//pusho table (in questo caso quella globale)
	lua_pushglobaltable(L);
	//pusho la chiave che voglio
	lua_pushstring(L, "matteo_key");
	//prendo la table a -2 nello stack, lui poi prender� la chiave a -1. risoltato viene messo in cima allo stack
	lua_gettable(L, -2);
	std::cout << (lua_tostring(L, -1)) << ", stack top count: " << lua_gettop(L) << ", stack top type: " << luaL_typename(L, -1) << std::endl;

	lua_pop(L, 2);

	//creare nuova tabella nella global table
	lua_pushglobaltable(L);//pusha global table
	lua_pushstring(L, "stronzi"); //pusha key della nuova table
	lua_newtable(L); //pusha tabella vuota
	lua_settable(L, -3);

	//scorciatoia, pusha il valore trovato con la chiave data e la table trovata al dato indice
	lua_getfield(L, -1, "stronzi");

	lua_pushstring(L, "matteo_key");
	lua_pushstring(L, "matteo_value");
	lua_settable(L, -3);//settata chiave e relativo valore nella table stronzi, poppate chiave e valore
	lua_pop(L, -2);

	//destroyed lua VM
	lua_close(L);
	*/
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

		world->destroyEntity(player);

		//world->tryAddSystem(engineECS::System::create<engineECS::RendererComponent>([](const engineECS::Entity& inEntity, const float deltaTime)
		//{
		//	std::cout << "Hello from " << inEntity.id << " entity with " << deltaTime << " delta time! (" << (deltaTime == 0.f ? -1 : 1.f / deltaTime) << ")" << std::endl;
		//}));

		world->tryAddSystem(engineECS::System::create<engineECS::TransformComponent, engineECS::RotatorComponent>([](const engineECS::Entity& inEntity, const float deltaTime)
		{
			engineECS::TransformComponent& transform = inEntity.getComponent<engineECS::TransformComponent>();
			engineECS::RotatorComponent& rotator = inEntity.getComponent<engineECS::RotatorComponent>();
			transform.rotation += engineECS::Vector3(rotator.rotationSpeed) * deltaTime;
			std::cout << "Hello from " << inEntity.id << " entity with " << deltaTime << " delta time! (" << (deltaTime == 0.f ? -1 : 1.f / deltaTime) << ")." << transform << std::endl;
		}));
	}

	while (true)
	{
		GET_ENGINE.run(0.01666666666f);
	}

	std::getchar();
	return 0;
}