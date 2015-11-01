#pragma once

#include "Enum.h"
#include <GrowingArray.h>

namespace Easy3D
{
	class Scene;
}

class CollisionManager;
class Entity;
class Player;

class EntityManager
{
public:
	EntityManager(Easy3D::Scene* aScene, CollisionManager* aCollisionManager);
	~EntityManager();

	Entity* CreateEntity(const std::string& aFilePath, eEntityType aType);

	void Update(float aDelta);

	Entity* FindClosestPickable(const CU::Vector3<float>& aPosition);

private:
	void operator=(EntityManager&) = delete;
	CU::GrowingArray<Entity*> myEntities;
	CU::GrowingArray<Entity*> myPickables;
	Player* myPlayer;

	Easy3D::Scene& myScene;
	CollisionManager& myCollisionManager;
};

