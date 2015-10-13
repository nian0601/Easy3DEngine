#pragma once

#include "Enum.h"
#include <string>

namespace luabridge
{
	class LuaRef;
	struct lua_State;
}

class Entity;

struct ChangeColorNote;
struct UpdateVelocityNote;

class Component
{
public:
	Component(Entity& aEntity, eComponent aType);
	virtual ~Component();

	virtual void RegisterClassToLua(luabridge::lua_State* aState);
	virtual void LoadFromScript(luabridge::LuaRef& aTableRef) = 0;

	virtual void Update(float aDelta) = 0;

	virtual void ReceiveNote(const ChangeColorNote& aNote);
	virtual void ReceiveNote(const UpdateVelocityNote& aNote);

	eComponent GetType() const;

protected:
	void operator=(Component&) = delete;


	const eComponent myType;
	Entity& myEntity;
};

inline eComponent Component::GetType() const
{
	return myType;
}