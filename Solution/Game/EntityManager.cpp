#include "stdafx.h"

#include "CollisionComponent.h"
#include "CollisionManager.h"
#include "EmitterComponent.h"
#include "Entity.h"
#include "GraphicsComponent.h"
#include "EntityManager.h"
#include <Scene.h>


EntityManager::EntityManager(Easy3D::Scene* aScene, CollisionManager* aCollisionManager)
	: myScene(*aScene)
	, myCollisionManager(*aCollisionManager)
	, myEntities(16)
{
}


EntityManager::~EntityManager()
{
}

Entity* EntityManager::CreateEntity(const std::string& aFilePath)
{
	Entity* newEntity = new Entity();
	newEntity->LoadFromScript(aFilePath);
	myEntities.Add(newEntity);


	CollisionComponent* comp = static_cast<CollisionComponent*>(newEntity->GetComponent(eComponent::COLLISION));
	if (comp != nullptr)
	{
		myCollisionManager.Add(comp);
	}

	GraphicsComponent* gfx = static_cast<GraphicsComponent*>(newEntity->GetComponent(eComponent::GRAPHIC));
	if (gfx != nullptr)
	{
		if (gfx->GetInstance() != nullptr)
		{
			myScene.AddInstance(gfx->GetInstance());
		}
	}

	EmitterComponent* emitter = static_cast<EmitterComponent*>(newEntity->GetComponent(eComponent::EMITTER));
	if (emitter != nullptr)
	{
		myScene.AddInstance(emitter->GetEmitter());
	}

	return newEntity;
}

void EntityManager::Update(float aDelta)
{
	for (int i = myEntities.Size() - 1; i >= 0; --i)
	{
		if (myEntities[i]->IsAlive() == false)
		{
			myEntities.DeleteCyclicAtIndex(i);
			continue;
		}

		myEntities[i]->Update(aDelta);
	}
}