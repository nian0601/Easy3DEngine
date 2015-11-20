#include "stdafx.h"
#include "InputComponent.h"
#include <InputWrapper.h>
#include <LuaBridge.h>
#include "ToggleInputNote.h"

InputComponent::InputComponent(Entity& aEntity)
	: Component(aEntity, eComponent::INPUT)
	, myInputEnabled(true)
{
}


InputComponent::~InputComponent()
{
}

void InputComponent::LoadFromScript(luabridge::LuaRef& aTableRef)
{
	using namespace luabridge;
	myWCommand = std::make_shared<LuaRef>(aTableRef["onWDown"]);
	if (myWCommand->isNil())
	{
		DL_ASSERT("Failed to Load InputComponent from script. Did not find \"onWDown\" in lua.");
	}

	mySCommand = std::make_shared<LuaRef>(aTableRef["onSDown"]);
	if (mySCommand->isNil())
	{
		DL_ASSERT("Failed to Load InputComponent from script. Did not find \"onSDown\" in lua.");
	}

	myACommand = std::make_shared<LuaRef>(aTableRef["onADown"]);
	if (myACommand->isNil())
	{
		DL_ASSERT("Failed to Load InputComponent from script. Did not find \"onADown\" in lua.");
	}

	myDCommand = std::make_shared<LuaRef>(aTableRef["onDDown"]);
	if (myDCommand->isNil())
	{
		DL_ASSERT("Failed to Load InputComponent from script. Did not find \"onDDown\" in lua.");
	}
}

void InputComponent::Update(float)
{
	if (myInputEnabled == false)
	{
		return;
	}

	if (CU::InputWrapper::GetInstance()->KeyIsPressed(DIK_W))
	{
		try{
			(*myWCommand)(&myEntity);
		}
		catch (luabridge::LuaException const& e) {
			DL_DEBUG("InpuComponent failed to call myWCommand. Lua Error: %s", e.what());
			DL_ASSERT("LUA ERROR! Check Debuglog.!");
		}
	}

	if (CU::InputWrapper::GetInstance()->KeyIsPressed(DIK_S))
	{
		try{
			(*mySCommand)(&myEntity);
		}
		catch (luabridge::LuaException const& e) {
			DL_DEBUG("InpuComponent failed to call mySCommand. Lua Error: %s", e.what());
			DL_ASSERT("LUA ERROR! Check Debuglog.!");
		}
	}

	if (CU::InputWrapper::GetInstance()->KeyIsPressed(DIK_A))
	{
		try{
			(*myACommand)(&myEntity);
		}
		catch (luabridge::LuaException const& e) {
			DL_DEBUG("InpuComponent failed to call myACommand. Lua Error: %s", e.what());
			DL_ASSERT("LUA ERROR! Check Debuglog.!");
		}
	}

	if (CU::InputWrapper::GetInstance()->KeyIsPressed(DIK_D))
	{
		try{
			(*myDCommand)(&myEntity);
		}
		catch (luabridge::LuaException const& e) {
			DL_DEBUG("InpuComponent failed to call myDCommand. Lua Error: %s", e.what());
			DL_ASSERT("LUA ERROR! Check Debuglog.!");
		}
	}
}

void InputComponent::ReceiveNote(const ToggleInputNote& aNote)
{
	myInputEnabled = aNote.myInputEnabled;
}

