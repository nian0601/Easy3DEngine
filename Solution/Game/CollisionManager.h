#pragma once

#include <GrowingArray.h>
class CollisionComponent;

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

	void CheckCollisions();
	void CleanUp(); //Call at the start of the frame to clean up stuff that died the last frame

	void Add(CollisionComponent* aComponent);

private:
	CU::GrowingArray<CollisionComponent*> myCollisionComponents;
};

