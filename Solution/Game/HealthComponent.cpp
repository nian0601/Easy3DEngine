#include "stdafx.h"

#include "Entity.h"
#include "HealthComponent.h"
#include <LuaBridge.h>

HealthComponent::HealthComponent(Entity& aEntity)
	: Component(aEntity, eComponent::HEALTH)
{
}


HealthComponent::~HealthComponent()
{
}

void HealthComponent::LoadFromScript(luabridge::LuaRef& aTableRef)
{
	using namespace luabridge;

	LuaRef healthRef = aTableRef["health"];
	if (healthRef.isNil())
	{
		DL_ASSERT("Failed to Load HealthComponent from script. Did not find \"health\" in lua.");
	}

	myMaxHealth = healthRef.cast<float>();
	myHealth = myMaxHealth;
}

void HealthComponent::Update(float aDelta)
{
	aDelta;
}

float HealthComponent::GetHealth() const
{
	return myHealth;
}

void HealthComponent::TakeDamage(float aDamage)
{
	myHealth -= aDamage;
	if (myHealth <= 0.f)
	{
		myEntity.Kill();
	}
}

void HealthComponent::Heal(float aAmount)
{
	myHealth += aAmount;
	if (myHealth > myMaxHealth)
	{
		myHealth = myMaxHealth;
	}
}

float HealthComponent::GetHealthPercent() const
{
	return myHealth / myMaxHealth;
}