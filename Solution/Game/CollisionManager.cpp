#include "stdafx.h"

#include "CollisionManager.h"
#include "CollisionComponent.h"
#include "Entity.h"

CollisionManager::CollisionManager()
	: myCollisionComponents(8)
{
}


CollisionManager::~CollisionManager()
{
}

void CollisionManager::CheckCollisions()
{
	for (int i = 0; i < myCollisionComponents.Size(); ++i)
	{
		for (int j = i + 1; j < myCollisionComponents.Size(); ++j)
		{
			float dist = CU::Length(myCollisionComponents[i]->GetPosition() - myCollisionComponents[j]->GetPosition());
			float rad = myCollisionComponents[i]->GetRadius() + myCollisionComponents[j]->GetRadius();

			if (dist < rad)
			{
				myCollisionComponents[i]->OnCollision(myCollisionComponents[j]->GetEntity());
				myCollisionComponents[j]->OnCollision(myCollisionComponents[i]->GetEntity());
			}
		}
	}
}

void CollisionManager::CleanUp()
{
	for (int i = myCollisionComponents.Size() - 1; i >= 0; --i)
	{
		if (myCollisionComponents[i]->GetEntity().IsAlive() == false)
		{
			myCollisionComponents.RemoveCyclicAtIndex(i);
		}
	}
}

void CollisionManager::Add(CollisionComponent* aComponent)
{
	myCollisionComponents.Add(aComponent);
}
