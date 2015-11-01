#include "stdafx.h"
#include "EmitterComponent.h"


EmitterComponent::EmitterComponent(Entity& aEntity)
	: Component(aEntity, eComponent::EMITTER)
{
}


EmitterComponent::~EmitterComponent()
{
}
