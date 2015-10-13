#include "stdafx.h"

#include <LuaBridge.h>
#include "ScriptComponent.h"


ScriptComponent::ScriptComponent(Entity& aEntity)
	: Component(aEntity, eComponent::SCRIPT)
{
}


ScriptComponent::~ScriptComponent()
{
}

void ScriptComponent::LoadFromScript(luabridge::LuaRef& aTableRef)
{
	using namespace luabridge;
	myOnUpdateCommand = std::make_shared<LuaRef>(aTableRef["onUpdate"]);
	if (myOnUpdateCommand->isNil())
	{
		DL_ASSERT("Failed to Load ScriptComponent from script. Did not find \"onUpdate\" in lua.");
	}
}

void ScriptComponent::Update(float aDelta)
{
	try{
		(*myOnUpdateCommand)(&myEntity, aDelta);
	}
	catch (luabridge::LuaException const& e) {
		DL_DEBUG("ScriptComponent failed to call myOnUpdateCommand. Lua Error: %s", e.what());
		DL_ASSERT("LUA ERROR! Check Debuglog.!");
	}
}
