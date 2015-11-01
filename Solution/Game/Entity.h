#pragma once

#include "Component.h"
#include "Enum.h"
#include <Matrix.h>
#include <unordered_map>

namespace luabridge
{
	struct lua_State;
	class LuaRef;
}

class Entity
{
public:
	Entity(eEntityType aType);
	~Entity();

	void LoadFromScript(const std::string& aScript);
	void AddComponent(Component* aComponent);
	Component* GetComponent(eComponent aComponent);

	virtual void Update(float aDeltaTime);

	eEntityType GetType() const;

	const CU::Matrix44<float>& GetOrientation() const;
	CU::Vector3<float> GetPosition() const;

	void SetPosition(const CU::Vector3<float>& aPosition);
	void SetPositionFromLua(float aX, float aY, float aZ);

	void Rotate(const CU::Matrix44<float>& aRotation);

	bool IsAlive() const;
	void Kill();

	const bool& IsActive() const;
	void Activate();
	void Deactivate();

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
	bool myIsActive;
	eEntityType myType;
};

inline eEntityType Entity::GetType() const
{
	return myType;
}

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

inline const bool& Entity::IsActive() const
{
	return myIsActive;
}

inline void Entity::Activate()
{
	DL_ASSERT_EXP(myIsActive == false, "Tried to activáte an allready active entity");
	myIsActive = true;
}

inline void Entity::Deactivate()
{
	DL_ASSERT_EXP(myIsActive == true, "Tried to deactivate an allready inactive entity");
	myIsActive = false;
}


template <typename T>
void Entity::SendNote(const T& aNote)
{
	for (int i = 0; i < myComponentsArray.Size(); ++i)
	{
		myComponentsArray[i]->ReceiveNote(aNote);
	}
}