#include "stdafx.h"
#include "Font.h"
#include "TextureContainer.h"


Easy3D::Font::Font()
	: myCharSize(32.f, 32.f)
{
}

bool Easy3D::Font::Init(const char* aFontPath)
{
	myTexture = TextureContainer::GetInstance()->GetTexture(aFontPath);

	return true;
}

Easy3D::Font::CharacterData Easy3D::Font::GetCharData(char aChar)
{
	int x = aChar % 16;
	int y = aChar / 16;

	
	CU::Vector2<float> topLeft(x * myCharSize.x, y * myCharSize.y);
	CU::Vector2<float> bottomLeft(topLeft.x + myCharSize.x, topLeft.y + myCharSize.y);


	CharacterData data;
	data.myTopLeftUV = topLeft / 512.f;
	data.myBottomRightUV = bottomLeft / 512.f;

	return data;
}