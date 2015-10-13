#pragma once

#include "Component.h"
namespace Easy3D
{
	class Sprite;
	class Instance;
}

class GraphicsComponent : public Component
{
public:
	GraphicsComponent(Entity& aEntity);
	~GraphicsComponent();

	void RegisterClassToLua(luabridge::lua_State* aState) override;
	void LoadFromScript(luabridge::LuaRef& aTableRef) override;

	void Update(float aDelta) override;

	void ReceiveNote(const ChangeColorNote& aNote) override;

	Easy3D::Instance* GetInstance() const;

private:
	void LoadSprite(luabridge::LuaRef& aSpriteRef);
	void LoadModel(luabridge::LuaRef& aModelRef);

	std::string myFilePath;
	Easy3D::Sprite* mySprite;
	Easy3D::Instance* myModel;
	CU::Vector4<float> myColor;
};

inline Easy3D::Instance* GraphicsComponent::GetInstance() const
{
	return myModel;
}
