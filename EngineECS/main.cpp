#include <memory>
#include <iostream>
#include <vector>
#include <bitset>
#include <functional>
#include <queue>

#define MAX_COMPONENTS 64

typedef unsigned int TypeId;

class World;

class Entity
{
private:
	Entity(World& InWorld, unsigned int InId) : Id(InId), World(InWorld)
	{

	}
public:
	const unsigned int Id;

	template<typename T>
	T& AddComponent()
	{
		return World.AddComponent<T>(*this);
	}

	template<typename T>
	T& GetComponent() const
	{
		return World.GetComponent<T>(*this);
	}

	friend std::ostream& operator<<(std::ostream& LHS, const Entity& RHS)
	{
		return LHS << RHS.Id;
	}

	World& World;
	friend class World;
};

namespace aiv
{
	static TypeId TypeIdCounter = 0;
}

struct EntityComponentMask
{
	std::bitset<MAX_COMPONENTS> Mask;
};

template<typename T>
struct Component
{
	static TypeId GetTypeId()
	{
		static TypeId InternalTypeId = aiv::TypeIdCounter++;
		return InternalTypeId;
	}

	static std::vector<T>& GetVector()
	{
		static std::vector<T> Vector;
		return Vector;
	}

protected:
	Component() = default;
};

struct PhysicsComponent : Component<PhysicsComponent>

{
	float mass;
};

struct RenderComponent : Component<RenderComponent>
{
	float x;
	float y;
	unsigned int textureId;
};

struct KillerComponent : Component<KillerComponent>
{
	unsigned long long Experience;
};

class World
{
public:
	Entity CreateEntity()
	{
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

	void DestroyEntity(Entity& InEntity)
	{
		Recycler.push(InEntity.Id);
		Entities[InEntity.Id].Mask.none();
	}

	template<typename T>
	T& AddComponent(const Entity& InEntity)
	{
		std::vector<T>& ComponentVector = T::GetVector();
		if (InEntity.Id >= ComponentVector.size())
		{
			ComponentVector.resize(InEntity.Id + 1);
		}
		ComponentVector[InEntity.Id] = {};
		Entities[InEntity.Id].Mask.set(T::GetTypeId(), true);
		return ComponentVector[InEntity.Id];
	}

	template<typename T>
	T& GetComponent(const Entity& InEntity)
	{
		std::vector<T>& ComponentVector = T::GetVector();
		return ComponentVector[InEntity.Id];
	}

	template<typename... Components>
	void ForEach(std::function<void(const Entity& InEntity)> Callback)
	{
		std::bitset<MAX_COMPONENTS> Mask = 0;
		[&Mask](...) {}(Mask.set(Components::GetTypeId(), true)...);
		for (unsigned int i = 0; i < Entities.size(); i++)
		{
			if ((Entities[i].Mask & Mask) == Mask)
			{
				const Entity FoundEntity = Entity(*this, i);
				Callback(FoundEntity);
			}
		}
	}

private:
	std::vector<EntityComponentMask> Entities;

	std::queue<unsigned int> Recycler;
};

int main(int argc, char **argv)
{

	World world;

	Entity player = world.CreateEntity();
	Entity enemy001 = world.CreateEntity();
	Entity enemy002 = world.CreateEntity();

	player.AddComponent<RenderComponent>();
	player.AddComponent<RenderComponent>();
	player.AddComponent<PhysicsComponent>();
	enemy002.AddComponent<RenderComponent>();
	player.AddComponent<PhysicsComponent>();

	world.DestroyEntity(player);

	world.ForEach<RenderComponent>([](const Entity& InEntity) {
		auto Renderer = InEntity.GetComponent<RenderComponent>();
		Renderer.x++;
	});

	std::getchar();
	return 0;
}