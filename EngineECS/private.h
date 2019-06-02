#pragma once

#define USE_ARRAY_ENTITIES
//#undef USE_ARRAY_ENTITIES

#define USE_ARRAY_SYSTEMS
//#undef USE_ARRAY_SYSTEMS

#define USE_ARRAY_COMPONENTS
//#undef USE_ARRAY_COMPONENTS

#define USE_ARRAY_WORLDS
//#undef USE_ARRAY_WORLDS

typedef int TypeId;
typedef int WorldIndex;

#define GET_ENGINE engineECS::Engine::getEngine()
#define GET_CURRENT_WORLD GET_ENGINE.getCurrentWorld()