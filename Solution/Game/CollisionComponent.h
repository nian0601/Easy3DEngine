#pragma once

#include "Component.h"
#include <memory>

namespace luabridge
{
	class LuaRef;
}

class CollisionComponent : public Component
{
public:
	CollisionComponent(Entity& aEntity);
	~CollisionComponent();

	void LoadFromScript(luabridge::LuaRef& aTableRef) override;
	void Update(float aDelta) override;

	void OnCollision(Entity& aOtherEntity);

	Entity& GetEntity();
	CU::Vector3<float> GetPosition() const;
	float GetRadius() const;

private:
	float myRadius;
	CU::Vector3<float> myPositionOffset;
	std::shared_ptr<luabridge::LuaRef> myOnCollision;
};