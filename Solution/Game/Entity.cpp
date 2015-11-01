#include "stdafx.h"

#include "ChangeColorNote.h"
#include "Component.h"
#include "CollisionComponent.h"
#include "EmitterComponent.h"
#include "Entity.h"
#include "GraphicsComponent.h"
#include "HealthComponent.h"
#include "InputComponent.h"
#include <LuaBridge.h>
#include "PhycisComponent.h"
#include "ScriptComponent.h"
#include "UpdateVelocityNote.h"



Entity::Entity(eEntityType aType)
	: myComponentsArray(8)
	, myAlive(true)
	, myIsActive(true)
	, myType(aType)
{
}


Entity::~Entity()
{
}

void Entity::LoadFromScript(const std::string& aScript)
{
	using namespace luabridge;
	lua_State* L = luaL_newstate();

	luaL_openlibs(L);
	RegisterClassToLua(L);
	luaL_dofile(L, aScript.c_str());
	LuaRef entity = getGlobal(L, "entity");

	LuaRef graphicsComponent = entity["graphicsComponent"];
	if (graphicsComponent.isNil() == false)
	{
		GraphicsComponent* gfx = new GraphicsComponent(*this);
		AddComponent(gfx);
	}

	LuaRef inputComponent = entity["inputComponent"];
	if (inputComponent.isNil() == false)
	{
		InputComponent* input = new InputComponent(*this);
		AddComponent(input);
	}

	LuaRef physicsComponent = entity["physicsComponent"];
	if (physicsComponent.isNil() == false)
	{
		PhycisComponent* phys = new PhycisComponent(*this);
		AddComponent(phys);
	}

	LuaRef scriptComponent = entity["scriptComponent"];
	if (scriptComponent.isNil() == false)
	{
		ScriptComponent* script = new ScriptComponent(*this);
		AddComponent(script);
	}

	LuaRef collisionComponent = entity["collisionComponent"];
	if (collisionComponent.isNil() == false)
	{
		CollisionComponent* collision = new CollisionComponent(*this);
		AddComponent(collision);
	}

	LuaRef healthComponent = entity["healthComponent"];
	if (healthComponent.isNil() == false)
	{
		HealthComponent* health = new HealthComponent(*this);
		AddComponent(health);
	}
	
	LuaRef emitterComponent = entity["emitterComponent"];
	if (emitterComponent.isNil() == false)
	{
		EmitterComponent* emitter = new EmitterComponent(*this);
		AddComponent(emitter);
	}


	

	for (int i = 0; i < myComponentsArray.Size(); ++i)
	{
		Component* comp = myComponentsArray[i];

		comp->RegisterClassToLua(L);
		switch (comp->GetType())
		{
		case eComponent::GRAPHIC:
			comp->LoadFromScript(graphicsComponent);
			break;
		case eComponent::INPUT:
			comp->LoadFromScript(inputComponent);
			break;
		case eComponent::PHYSICS:
			comp->LoadFromScript(physicsComponent);
			break;
		case eComponent::SCRIPT:
			comp->LoadFromScript(scriptComponent);
			break;
		case eComponent::COLLISION:
			comp->LoadFromScript(collisionComponent);
			break;
		case eComponent::HEALTH:
			comp->LoadFromScript(healthComponent);
			break;
		case eComponent::EMITTER:
			comp->LoadFromScript(emitterComponent);
			break;
		default:
			break;
		}
	}
}

void Entity::AddComponent(Component* aComponent)
{
	auto it = myComponentsMap.find(aComponent->GetType());

	if (it == myComponentsMap.end())
	{
		myComponentsMap[aComponent->GetType()] = aComponent;
		myComponentsArray.Add(aComponent);
	}
}

Component* Entity::GetComponent(eComponent aComponent)
{
	auto it = myComponentsMap.find(aComponent);

	if (it != myComponentsMap.end())
	{
		return it->second;
	}

	return nullptr;
}

void Entity::Update(float aDeltaTime)
{
	for (int i = 0; i < myComponentsArray.Size(); ++i)
	{
		myComponentsArray[i]->Update(aDeltaTime);
	}
}

void Entity::Rotate(const CU::Matrix44<float>& aRotation)
{
	CU::Vector3<float> pos = myOrientation.GetPos();
	myOrientation.SetPos({ 0.f, 0.f, 0.f, 1.f });
	myOrientation = aRotation * myOrientation;
	myOrientation.SetPos(pos);
}

void Entity::TakeDamage(float aAmount)
{
	Component* health = GetComponent(eComponent::HEALTH);
	if (health != nullptr)
	{
		reinterpret_cast<HealthComponent*>(health)->TakeDamage(aAmount);
	}
}

float Entity::GetHealth()
{
	Component* health = GetComponent(eComponent::HEALTH);
	if (health != nullptr)
	{
		return reinterpret_cast<HealthComponent*>(health)->GetHealth();
	}

	return -1.f;
}

void Entity::StopMovement()
{
	Component* physics = GetComponent(eComponent::PHYSICS);
	if (physics != nullptr)
	{
		reinterpret_cast<PhycisComponent*>(physics)->StopMovement();
	}
}

void Entity::SendLUAMessage(luabridge::LuaRef aMsg)
{
	using namespace luabridge;
	LuaRef type = aMsg["type"];

	DL_ASSERT_EXP(type.isNil() == false, "Entity recieved a LUAMessage without a \"type\"");
	DL_ASSERT_EXP(type.isString() == true, "Entity recieved a LUAMessage where \"type\" wasnt a string");

	std::string typeString = type.cast<std::string>();

	if (typeString == "color")
	{
		LuaRef r = aMsg["r"];
		LuaRef g = aMsg["g"];
		LuaRef b = aMsg["b"];
		LuaRef a = aMsg["a"];

		DL_ASSERT_EXP(r.isNil() == false && r.isNumber() == true, "Failed to read \"r\" from colormessage.");
		DL_ASSERT_EXP(g.isNil() == false && g.isNumber() == true, "Failed to read \"g\" from colormessage.");
		DL_ASSERT_EXP(b.isNil() == false && b.isNumber() == true, "Failed to read \"b\" from colormessage.");
		DL_ASSERT_EXP(a.isNil() == false && a.isNumber() == true, "Failed to read \"a\" from colormessage.");

		CU::Vector4<float> color;
		color.r = r.cast<float>();
		color.g = g.cast<float>();
		color.b = b.cast<float>();
		color.a = a.cast<float>();

		ChangeColorNote note(color);
		SendNote(note);
	}
	if (typeString == "move")
	{
		LuaRef x = aMsg["x"];
		LuaRef y = aMsg["y"];
		LuaRef z = aMsg["z"];

		DL_ASSERT_EXP(x.isNil() == false && x.isNumber() == true, "Failed to read \"x\" from movemessage.");
		DL_ASSERT_EXP(y.isNil() == false && y.isNumber() == true, "Failed to read \"y\" from movemessage.");
		DL_ASSERT_EXP(z.isNil() == false && z.isNumber() == true, "Failed to read \"z\" from movemessage.");

		CU::Vector3<float> direction;
		direction.x = x.cast<float>();
		direction.y = y.cast<float>();
		direction.z = z.cast<float>();

		UpdateVelocityNote note(direction);
		SendNote(note);
	}
}

void Entity::RegisterClassToLua(luabridge::lua_State* aState)
{
	getGlobalNamespace(aState)
		.beginClass<Entity>("Entity")
		.addFunction("SendLuaMessage", &Entity::SendLUAMessage)
		.addFunction("TakeDamage", &Entity::TakeDamage)
		.addFunction("GetHealth", &Entity::GetHealth)
		.addFunction("SetPosition", &Entity::SetPositionFromLua)
		.addFunction("StopMovement", &Entity::StopMovement)
		.endClass();
}
