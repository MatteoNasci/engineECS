#include "World.h"

engineECS::Entity engineECS::World::CreateEntity() {
	unsigned int Id = 0;
	if (Recycler.empty())
	{
		Entities.push_back(EntityComponentMask());
		Id = static_cast<unsigned int>(Entities.size()) - 1;
	}
	else
	{
		Id = Recycler.front();
		Recycler.pop();
	}
	return Entity(*this, Id);
}

void engineECS::World::DestroyEntity(const engineECS::Entity& InEntity)
{
	Recycler.push(InEntity.Id);
	Entities[InEntity.Id].Mask.reset();
}

void engineECS::World::ForEachAll(const std::function<void(const engineECS::Entity& InEntity)>& Callback)
{
	for (unsigned int i = 0; i < Entities.size(); i++)
	{
		if ((Entities[i].Mask.any()))
		{
			Callback(Entity(*this, i));
		}
	}
}