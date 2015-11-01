#include "stdafx.h"

#include "ChangeColorNote.h"
#include "EntityManager.h"
#include <InputWrapper.h>
#include "Player.h"


Player::Player(eEntityType aType, EntityManager& aEntityManager)
	: Entity(aType)
	, myEntityManager(aEntityManager)
	, myInventory(8)
{
}


Player::~Player()
{
}

void Player::Update(float aDelta)
{
	Entity::Update(aDelta);

	Entity* closest = myEntityManager.FindClosestPickable(GetPosition());

	if (closest != nullptr)
	{
		closest->SendNote<ChangeColorNote>(ChangeColorNote({ 0.f, 1.f, 1.f, 1.f }));

		if (CU::InputWrapper::GetInstance()->MouseDown(0))
		{
			myInventory.Add(closest);
			myInventory.GetLast()->Deactivate();
		}
	}

	if (CU::InputWrapper::GetInstance()->MouseDown(1) && myInventory.Size() > 0)
	{
		Entity* ent = myInventory[0];
		myInventory.RemoveCyclicAtIndex(0);

		ent->Activate();
		ent->SetPosition(GetPosition());
		ent = nullptr;
	}
}
