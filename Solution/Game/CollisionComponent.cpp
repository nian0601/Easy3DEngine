#include "stdafx.h"

#include "CollisionComponent.h"
#include "Entity.h"
#include <LuaBridge.h>

CollisionComponent::CollisionComponent(Entity& aEntity)
	: Component(aEntity, eComponent::COLLISION)
{
}


CollisionComponent::~CollisionComponent()
{
}

void CollisionComponent::LoadFromScript(luabridge::LuaRef& aTableRef)
{
	using namespace luabridge;

	LuaRef radius = aTableRef["radius"];
	if (radius.isNil())
	{
		DL_ASSERT("Failed to Load CollisionComponent from script. Did not find \"radius\" in lua.");
	}
	myRadius = radius.cast<float>();

	LuaRef offsetRef = aTableRef["offset"];
	if (offsetRef.isNil() == false)
	{
		LuaRef xRef = offsetRef["x"];
		LuaRef yRef = offsetRef["y"];
		LuaRef zRef = offsetRef["z"];

		if (xRef.isNil() == true || yRef.isNil() == true || zRef.isNil() == true)
		{
			DL_ASSERT("Failed to Load CollisionComponent from script. Did not find \"x\\y\\z\" in offset in lua.");
		}

		myPositionOffset.x = xRef.cast<float>();
		myPositionOffset.y = yRef.cast<float>();
		myPositionOffset.z = zRef.cast<float>();
	}


	myOnCollision = std::make_shared<LuaRef>(aTableRef["onCollision"]);
	if (myOnCollision->isNil())
	{
		DL_ASSERT("Failed to Load CollisionComponent from script. Did not find \"onCollision\" in lua.");
	}
}

void CollisionComponent::Update(float aDelta)
{
	aDelta;
}

void CollisionComponent::OnCollision(Entity& aOtherEntity)
{
	try{
		(*myOnCollision)(&myEntity, &aOtherEntity);
	}
	catch (luabridge::LuaException const& e) {
		DL_DEBUG("CollisionComponent failed to call myOnCollision. Lua Error: %s", e.what());
		DL_ASSERT("LUA ERROR! Check Debuglog.!");
	}
}

Entity& CollisionComponent::GetEntity()
{
	return myEntity;
}

CU::Vector3<float> CollisionComponent::GetPosition() const
{
	return myEntity.GetPosition() + myPositionOffset;
}

float CollisionComponent::GetRadius() const
{
	return myRadius;
}