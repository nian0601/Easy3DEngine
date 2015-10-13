#include "stdafx.h"
#include "Component.h"


Component::Component(Entity& aEntity, eComponent aType)
	: myEntity(aEntity)
	, myType(aType)
{
}


Component::~Component()
{
}

void Component::RegisterClassToLua(luabridge::lua_State*)
{
}

void Component::ReceiveNote(const ChangeColorNote&)
{
}

void Component::ReceiveNote(const UpdateVelocityNote&)
{
}
