#include "stdafx.h"
#include "FontContainer.h"
#include "Font.h"


Easy3D::FontContainer::~FontContainer()
{
	for (auto it = myFonts.begin(); it != myFonts.end(); ++it)
	{
		delete it->second;
		it->second = nullptr;
	}
}

Easy3D::Font* Easy3D::FontContainer::GetFont(const std::string& aPath)
{
	auto it = myFonts.find(aPath);

	if (it == myFonts.end())
	{
		LoadFont(aPath);
	}

	return myFonts[aPath];
}

void Easy3D::FontContainer::LoadFont(const std::string& aPath)
{
	Font* newFont = new Font();
	newFont->Init(aPath.c_str());

	myFonts[aPath] = newFont;
}