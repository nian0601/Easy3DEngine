#pragma once

#include "Enum.h"
#include <Matrix.h>
#include <unordered_map>

namespace luabridge
{
	struct lua_State;
	class LuaRef;
}

class Component;
class Entity
{
public:
	Entity();
	~Entity();

	void LoadFromScript(const std::string& aScript);
	void AddComponent(Component* aComponent);
	Component* GetComponent(eComponent aComponent);

	void Update(float aDeltaTime);

	const CU::Matrix44<float>& GetOrientation() const;
	CU::Vector3<float> GetPosition() const;
	void SetPosition(const CU::Vector3<float>& aPosition);
	void SetPositionFromLua(float aX, float aY, float aZ);
	void Rotate(const CU::Matrix44<float>& aRotation);

	bool IsAlive() const;
	void Kill();

	void TakeDamage(float aAmount);
	float GetHealth();

	void StopMovement();

	void SendLUAMessage(luabridge::LuaRef aMsg);

	template <typename T>
	void SendNote(const T& aNote);

private:
	std::unordered_map<eComponent, Component*> myComponentsMap;
	CU::GrowingArray<Component*> myComponentsArray;
	CU::Matrix44<float> myOrientation;

	void RegisterClassToLua(luabridge::lua_State* aState);

	bool myAlive;
};

inline const CU::Matrix44<float>& Entity::GetOrientation() const
{
	return myOrientation;
}

inline CU::Vector3<float> Entity::GetPosition() const
{
	return myOrientation.GetPos();
}

inline void Entity::SetPosition(const CU::Vector3<float>& aPosition)
{
	myOrientation.SetPos(aPosition);
}

inline void Entity::SetPositionFromLua(float aX, float aY, float aZ)
{
	myOrientation.SetPos(CU::Vector3<float>(aX, aY, aZ));
}

inline bool Entity::IsAlive() const
{
	return myAlive;
}

inline void Entity::Kill()
{
	myAlive = false;
}

template <typename T>
void Entity::SendNote(const T& aNote)
{
	for (int i = 0; i < myComponentsArray.Size(); ++i)
	{
		myComponentsArray[i]->ReceiveNote(aNote);
	}
}