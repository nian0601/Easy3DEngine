#include "stdafx.h"

#include "Texture.h"
#include "TextureContainer.h"


Easy3D::TextureContainer::~TextureContainer()
{
	for (auto it = myTextures.begin(); it != myTextures.end(); ++it)
	{
		delete it->second;
	}

	myTextures.clear();
}

Easy3D::Texture* Easy3D::TextureContainer::GetTexture(const std::string& aFileName)
{
	auto it = myTextures.find(aFileName);

	if (it == myTextures.end())
	{
		LoadTexture(aFileName);
	}

	return myTextures[aFileName];
}

void Easy3D::TextureContainer::LoadTexture(const std::string& aFileName)
{
	Texture* newTex = new Texture();
	newTex->LoadTexture(aFileName);

	myTextures[aFileName] = newTex;
}