#pragma once
#include "Component.h"

namespace Easy3D
{
	class ParticleEmitterInstance;
}

class EmitterComponent : public Component
{
public:
	EmitterComponent(Entity& aEntity);
	~EmitterComponent();

	void LoadFromScript(luabridge::LuaRef& aTableRef) override;

	void Update(float aDelta) override;

	Easy3D::ParticleEmitterInstance* GetEmitter() const;

private:
	Easy3D::ParticleEmitterInstance* myEmitter;
};

inline Easy3D::ParticleEmitterInstance* EmitterComponent::GetEmitter() const
{
	return myEmitter;
}