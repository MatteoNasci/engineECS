#pragma once

typedef int TypeId;
typedef int WorldIndex;

#define GET_ENGINE engineECS::Engine::getEngine()
#define GET_CURRENT_WORLD GET_ENGINE.getCurrentWorld()