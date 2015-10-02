#include "stdafx.h"

#include "Texture.h"
#include "TextureContainer.h"


Prism::TextureContainer::~TextureContainer()
{
	for (auto it = myTextures.begin(); it != myTextures.end(); ++it)
	{
		delete it->second;
	}

	myTextures.clear();
}

Prism::Texture* Prism::TextureContainer::GetTexture(const std::string& aFileName)
{
	auto it = myTextures.find(aFileName);

	if (it == myTextures.end())
	{
		LoadTexture(aFileName);
	}

	return myTextures[aFileName];
}

void Prism::TextureContainer::LoadTexture(const std::string& aFileName)
{
	Texture* newTex = new Texture();
	newTex->LoadTexture(aFileName);

	myTextures[aFileName] = newTex;
}