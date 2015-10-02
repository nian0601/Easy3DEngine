#include "stdafx.h"
#include "FontContainer.h"
#include "Font.h"

Prism::Font* Prism::FontContainer::GetFont(const std::string& aPath)
{
	auto it = myFonts.find(aPath);

	if (it == myFonts.end())
	{
		LoadFont(aPath);
	}

	return myFonts[aPath];
}

void Prism::FontContainer::LoadFont(const std::string& aPath)
{
	Font* newFont = new Font();
	newFont->Init(aPath.c_str());

	myFonts[aPath] = newFont;
}