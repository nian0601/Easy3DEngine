#pragma once

#include "Component.h"
#include <Vector.h>

class PhycisComponent : public Component
{
public:
	PhycisComponent(Entity& aEntity);
	~PhycisComponent();

	void LoadFromScript(luabridge::LuaRef& aTableRef) override;
	void Update(float aDelta) override;

	void ReceiveNote(const UpdateVelocityNote& aNote) override;

	void StopMovement();

private:
	bool myRecievedInput;

	CU::Vector3<float> myDirection;
	float myMoveSpeed;
	float myMaxMoveSpeed;
	float myAcceleration;
};

