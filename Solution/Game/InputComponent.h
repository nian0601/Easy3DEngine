#pragma once

#include "Component.h"
#include <memory>

namespace luabridge
{
	class LuaRef;
}

class InputComponent : public Component
{
public:
	InputComponent(Entity& aEntity);
	~InputComponent();

	void LoadFromScript(luabridge::LuaRef& aTableRef) override;
	void Update(float aDelta) override;

	void ReceiveNote(const ToggleInputNote& aNote) override;

private:
	std::shared_ptr<luabridge::LuaRef> myWCommand;
	std::shared_ptr<luabridge::LuaRef> mySCommand;
	std::shared_ptr<luabridge::LuaRef> myACommand;
	std::shared_ptr<luabridge::LuaRef> myDCommand;

	bool myInputEnabled;
};

