#include "stdafx.h"

#include "Entity.h"
#include "EmitterComponent.h"
#include <EmitterContainer.h>
#include <LuaBridge.h>
#include <ParticleEmitterInstance.h>


EmitterComponent::EmitterComponent(Entity& aEntity)
	: Component(aEntity, eComponent::EMITTER)
{
}


EmitterComponent::~EmitterComponent()
{
}

void EmitterComponent::LoadFromScript(luabridge::LuaRef& aTableRef)
{
	using namespace luabridge;

	LuaRef file = aTableRef["file"];
	if (file.isNil())
	{
		DL_ASSERT("Failed to Load EmitterComponent from script. Did not find \"file\" in lua.");
	}

	myEmitter = Easy3D::Engine::GetInstance()->GetEmitterContainer()->CreateEmitter(file.cast<std::string>());
}

void EmitterComponent::Update(float aDelta)
{
	myEmitter->SetPosition(myEntity.GetPosition());
	myEmitter->Update(aDelta);
}
