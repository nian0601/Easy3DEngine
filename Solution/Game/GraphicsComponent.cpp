#include "stdafx.h"

#include "ChangeColorNote.h"
#include <Engine.h>
#include "Entity.h"
#include "GraphicsComponent.h"
#include <Instance.h>
#include <LuaBridge.h>
#include <ModelLoader.h>
#include <SpriteInstance.h>
#include <Vector.h>


GraphicsComponent::GraphicsComponent(Entity& aEntity)
	: Component(aEntity, eComponent::GRAPHIC)
	, myColor(1.f, 1.f, 1.f, 1.f)
	, mySprite(nullptr)
	, myModel(nullptr)
{
}


GraphicsComponent::~GraphicsComponent()
{
}

void GraphicsComponent::RegisterClassToLua(luabridge::lua_State*)
{
}

void GraphicsComponent::LoadFromScript(luabridge::LuaRef& aTableRef)
{
	using namespace luabridge;

	LuaRef file = aTableRef["file"];
	if (file.isNil())
	{
		DL_ASSERT("Failed to Load GraphicsComponent from script. Did not find \"file\" in lua.");
	}
	myFilePath = file.cast<std::string>();

	LuaRef sprite = aTableRef["sprite"];
	LuaRef model = aTableRef["model"];

	if (sprite.isNil() == false && model.isNil() == false)
	{
		DL_ASSERT("Failed to Load GraphicsComponent from script. Found both \"sprite\" and \"model\" in lua, choose one.");
	}

	if (sprite.isNil() == true && model.isNil() == true)
	{
		DL_ASSERT("Failed to Load GraphicsComponent from script. Couldnt find either \"sprite\" or \"model\" in lua.");
	}

	if (sprite.isNil() == false)
	{
		LoadSprite(sprite);
	}
	else if (model.isNil() == false)
	{
		LoadModel(model);
	}
}

void GraphicsComponent::Update(float aDelta)
{
	aDelta;

	if (mySprite != nullptr)
	{
		CU::Vector2<float> pos = { myEntity.GetOrientation().GetPos().x, myEntity.GetOrientation().GetPos().y };
		mySprite->Render(pos, { 1.f, 1.f }, myColor);
	}
}

void GraphicsComponent::ReceiveNote(const ChangeColorNote& aNote)
{
	myColor = aNote.myColor;
	if (myModel != nullptr)
	{
		myModel->SetColor(myColor);
	}
}

void GraphicsComponent::LoadSprite(luabridge::LuaRef& aSpriteRef)
{
	using namespace luabridge;
	LuaRef sizeRef = aSpriteRef["size"];
	if (sizeRef.isNil())
	{
		DL_ASSERT("Failed to Load GraphicsComponent from script. Did not find \"size\" in lua.");
	}

	LuaRef widthRef = sizeRef["width"];
	LuaRef heightReg = sizeRef["height"];
	if (widthRef.isNil() == true || heightReg.isNil() == true)
	{
		DL_ASSERT("One or more elements (width/height) is missing from entity-script (graphicscomponent->size).");
	}

	CU::Vector2<float> size;
	size.x = widthRef.cast<float>();
	size.y = heightReg.cast<float>();


	CU::Vector2<float> hotspot;
	LuaRef hotspotRef = aSpriteRef["hotspot"];
	if (hotspotRef.isNil() == false)
	{
		LuaRef hotspotXRef = hotspotRef["width"];
		LuaRef hotspotYRef = hotspotRef["height"];

		if (hotspotXRef.isNil() == false && hotspotYRef.isNil() == false)
		{
			hotspot.x = hotspotXRef.cast<float>();
			hotspot.y = hotspotYRef.cast<float>();
		}
		else
		{
			DL_ASSERT("One or more elements (width/height) is missing from entity-script (graphicscomponent->hotspot).");
		}
	}

	mySprite = new Easy3D::SpriteInstance(myFilePath, size, hotspot);
}

void GraphicsComponent::LoadModel(luabridge::LuaRef& aModelRef)
{
	using namespace luabridge;
	LuaRef shaderRef = aModelRef["shader"];
	if (shaderRef.isNil())
	{
		DL_ASSERT("Failed to Load GraphicsComponent from script. Did not find \"shader\" in lua.");
	}
	std::string shader = shaderRef.cast<std::string>();

	myModel = new Easy3D::Instance(*Easy3D::ModelLoader::GetInstance()->LoadModel(myFilePath, shader)
		, myEntity.GetOrientation(), myEntity.IsActive());
}
