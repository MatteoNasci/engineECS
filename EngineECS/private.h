#pragma once
typedef size_t TypeId;
typedef size_t WorldIndex;

#define SYSTEM_FUNCTION std::function<void(const engineECS::Entity& inEntity)>
#define GET_ENGINE engineECS::Engine::getEngine()
#define GET_CURRENT_WORLD GET_ENGINE.getCurrentWorld()