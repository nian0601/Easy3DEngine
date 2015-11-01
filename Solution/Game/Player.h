#pragma once
#include "Entity.h"

class EntityManager;

class Player : public Entity
{
public:
	Player(eEntityType aType, EntityManager& aEntityManager);
	~Player();

	void Update(float aDelta) override;

private:
	void operator=(Player&) = delete;

	EntityManager& myEntityManager;
	CU::GrowingArray<Entity*> myInventory;
};

