
#include "System.h"
engineECS::System::System() : engineECS::System::System(nullptr)
{

}
//engineECS::System::System(const std::function<void(const engineECS::Entity& inEntity)>& inCallback) : callback(inCallback)
//{
//	//auto a = std::bind(&System::Execute, *this, std::placeholders::_1);
//	//std::function<void(engineECS::Engine& engine, engineECS::World& world)> b = a;
//	//World.AddSystem(b);
//}
engineECS::System::System(const std::function<void(const engineECS::Entity& inEntity, const float deltaTime)>& inCallback) : callback(inCallback)
{
	mask = {};
}
engineECS::System::System(const std::function<void(const engineECS::Entity& inEntity, const float deltaTime)>& inCallback, const engineECS::EntityComponentMask& inMask) : callback(inCallback), mask(inMask)
{

}
engineECS::System::System(const engineECS::System& other) : engineECS::System::System(other.callback, other.mask)
{

}
engineECS::System::System(engineECS::System&& other) : engineECS::System::System(other.callback, other.mask)
{

}
void engineECS::System::setMask(const engineECS::EntityComponentMask& mask)
{
	this->mask = mask;
}
engineECS::System& engineECS::System::operator=(const engineECS::System& other)
{
	callback = other.callback;
	mask = other.mask;
	return *this;
}
engineECS::System& engineECS::System::operator=(engineECS::System&& other)
{
	callback = other.callback;
	mask = other.mask;
	return *this;
}
const engineECS::EntityComponentMask& engineECS::System::getMask() const
{
	return mask;
}
const std::function<void(const engineECS::Entity& inEntity, const float deltaTime)>& engineECS::System::getCallback() const
{ 
	return callback;
}

engineECS::System::~System()
{
}
