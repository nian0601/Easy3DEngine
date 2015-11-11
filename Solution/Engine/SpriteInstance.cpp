#include "stdafx.h"

#include "Sprite.h"
#include "SpriteInstance.h"


namespace Easy3D
{
	SpriteInstance::SpriteInstance(const std::string& aSpritePath, const CU::Vector2<float>& aSize
			, const CU::Vector2<float>& aHotSpot)
		: mySprite(new Sprite(aSpritePath, aSize, aHotSpot))
	{
	}


	SpriteInstance::~SpriteInstance()
	{
		delete mySprite;
	}

	void SpriteInstance::Render(const CU::Vector2<float>& aPosition, const CU::Vector2<float>& aScale
		, const CU::Vector4<float>& aColor)
	{
		mySprite->Render(aPosition, aScale, aColor);
	}
}