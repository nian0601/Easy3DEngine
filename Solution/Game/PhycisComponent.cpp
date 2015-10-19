#include "stdafx.h"

#include "Entity.h"
#include <LuaBridge.h>
#include "PhycisComponent.h"
#include "UpdateVelocityNote.h"


PhycisComponent::PhycisComponent(Entity& aEntity)
	: Component(aEntity, eComponent::PHYSICS)
	, myMaxMoveSpeed(0.f)
	, myMoveSpeed(0.f)
	, myAcceleration(0.f)
	, myRecievedInput(false)
{
}


PhycisComponent::~PhycisComponent()
{
}

void PhycisComponent::LoadFromScript(luabridge::LuaRef& aTableRef)
{
	using namespace luabridge;

	LuaRef speed = aTableRef["speed"];
	if (speed.isNil())
	{
		DL_ASSERT("Failed to Load PhycisComponent from script. Did not find \"speed\" in lua.");
	}

	myMaxMoveSpeed = speed.cast<float>();
	myAcceleration = 10.f;
}

void PhycisComponent::Update(float aDelta)
{
	if (myRecievedInput)
	{
		myRecievedInput = false;
		myMoveSpeed += myAcceleration * aDelta;
	}
	else
	{
		myMoveSpeed -= myAcceleration * aDelta;
	}

	if (myMoveSpeed > myMaxMoveSpeed)
	{
		myMoveSpeed = myMaxMoveSpeed;
	}
	if (myMoveSpeed < 0.f)
	{
		myMoveSpeed = 0.f;
	}


	CU::Vector3<float> position = myEntity.GetPosition();
	position += myDirection * myMoveSpeed * aDelta;
	myEntity.SetPosition(position);
}

void PhycisComponent::ReceiveNote(const UpdateVelocityNote& aNote)
{
	myDirection += aNote.myDirection;
	CU::Normalize(myDirection);
	myRecievedInput = true;
}

void PhycisComponent::StopMovement()
{
	myMoveSpeed = 0.f;
}
