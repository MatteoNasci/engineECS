
#include "System.h"

engineECS::System::System(const std::function<void(const engineECS::Entity& inEntity)>& inCallback) : callback(inCallback)
{
	//auto a = std::bind(&System::Execute, *this, std::placeholders::_1);
	//std::function<void(engineECS::Engine& engine, engineECS::World& world)> b = a;
	//World.AddSystem(b);
}
const SYSTEM_FUNCTION& engineECS::System::getCallback() const
{ 
	return callback;
}

engineECS::System::~System()
{
}
