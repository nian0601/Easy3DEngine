#pragma once

#include <GrowingArray.h>

namespace Easy3D
{
	class Scene;
}

class CollisionManager;
class Entity;

class EntityManager
{
public:
	EntityManager(Easy3D::Scene* aScene, CollisionManager* aCollisionManager);
	~EntityManager();

	Entity* CreateEntity(const std::string& aFilePath);

	void Update(float aDelta);

private:
	void operator=(EntityManager&) = delete;
	CU::GrowingArray<Entity*> myEntities;

	Easy3D::Scene& myScene;
	CollisionManager& myCollisionManager;
};

