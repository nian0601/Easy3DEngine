#include "stdafx.h"

#include "ChangeColorNote.h"
#include "CollisionComponent.h"
#include "CollisionManager.h"
#include "EmitterComponent.h"
#include "Entity.h"
#include "GraphicsComponent.h"
#include "EntityManager.h"
#include "Player.h"
#include <Scene.h>


EntityManager::EntityManager(Easy3D::Scene* aScene, CollisionManager* aCollisionManager)
	: myScene(*aScene)
	, myCollisionManager(*aCollisionManager)
	, myEntities(16)
	, myPickables(8)
	, myPlayer(nullptr)
{
}


EntityManager::~EntityManager()
{
}

Entity* EntityManager::CreateEntity(const std::string& aFilePath, eEntityType aType)
{
	Entity* newEntity = nullptr;
	switch (aType)
	{
	case eEntityType::PLAYER:
		DL_ASSERT_EXP(myPlayer == nullptr, "Tried to create more than one player");
		newEntity = new Player(aType, *this);
		myPlayer = static_cast<Player*>(newEntity);
		break;
	case eEntityType::PICKABLE:
		newEntity = new Entity(aType);
		myPickables.Add(newEntity);
		break;
	}

	DL_ASSERT_EXP(newEntity != nullptr, "Tried to create entity of invalid type");

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

		if (myEntities[i]->IsActive() == false)
		{
			continue;
		}

		myEntities[i]->Update(aDelta);
	}
}

Entity* EntityManager::FindClosestPickable(const CU::Vector3<float>& aPosition)
{
	Entity* closest = nullptr;
	float bestDist = FLT_MAX;

	for (int i = 0; i < myPickables.Size(); ++i)
	{
		if (myPickables[i]->IsActive() == false)
		{
			continue;
		}

		myPickables[i]->SendNote<ChangeColorNote>(ChangeColorNote({ 1.f, 1.f, 1.f, 1.f }));
		float dist = CU::Length(myPickables[i]->GetPosition() - aPosition);
		if (dist < bestDist)
		{
			bestDist = dist;
			closest = myPickables[i];
		}
	}

	return closest;
}