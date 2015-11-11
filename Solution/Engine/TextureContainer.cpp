#include "stdafx.h"

#include "Texture.h"
#include "TextureContainer.h"

namespace Easy3D
{
	TextureContainer* TextureContainer::myInstance = nullptr;

	void TextureContainer::Create()
	{
		myInstance = new TextureContainer();
	}

	void TextureContainer::Destroy()
	{
		delete myInstance;
	}

	TextureContainer* TextureContainer::GetInstance()
	{
		DL_ASSERT_EXP(myInstance != nullptr, "TextureContainer: myInstance is nullptr, forgot to create?");
		return myInstance;
	}

	TextureContainer::~TextureContainer()
	{
		for (auto it = myTextures.begin(); it != myTextures.end(); ++it)
		{
			delete it->second;
		}

		myTextures.clear();
	}

	Texture* TextureContainer::GetTexture(const std::string& aFileName)
	{
		auto it = myTextures.find(aFileName);

		if (it == myTextures.end())
		{
			LoadTexture(aFileName);
		}

		return myTextures[aFileName];
	}

	void TextureContainer::LoadTexture(const std::string& aFileName)
	{
		Texture* newTex = new Texture();
		newTex->LoadTexture(aFileName);

		myTextures[aFileName] = newTex;
	}
}