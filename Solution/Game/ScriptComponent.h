#pragma once

#include "Component.h"
#include <memory>

namespace luabridge
{
	class LuaRef;
}

class ScriptComponent : public Component
{
public:
	ScriptComponent(Entity& aEntity);
	~ScriptComponent();

	void LoadFromScript(luabridge::LuaRef& aTableRef) override;
	void Update(float aDelta) override;

private:
	std::shared_ptr<luabridge::LuaRef> myOnUpdateCommand;
};

