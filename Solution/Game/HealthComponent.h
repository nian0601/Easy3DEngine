#pragma once

#include "Component.h"
class HealthComponent : public Component
{
public:
	HealthComponent(Entity& aEntity);
	~HealthComponent();

	void LoadFromScript(luabridge::LuaRef& aTableRef) override;
	void Update(float aDelta) override;

	float GetHealth() const;
	void TakeDamage(float aDamage);
	void Heal(float aAmount);
	float GetHealthPercent() const;

private:
	float myHealth;
	float myMaxHealth;
};