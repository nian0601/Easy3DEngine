#include "stdafx.h"

#include "Text.h"
#include "TextInstance.h"


namespace Easy3D
{
	TextInstance::TextInstance()
		: myText(nullptr)
	{
	}


	TextInstance::~TextInstance()
	{
		delete myText;
	}

	void TextInstance::Init(const std::string& aFontPath)
	{
		myText = new Text();
		myText->Init(aFontPath);
	}

	void TextInstance::Render(const std::string& aString, const CU::Vector2<float>& aPosition
		, const CU::Vector2<float>& aScale 
		, const CU::Vector4<float>& aColor)
	{
		myText->Render(aString, aPosition, aScale, aColor);
	}

	float TextInstance::GetTextWidth() const
	{
		return myText->GetTextWidth();
	}

	CU::Vector2<float> TextInstance::GetTextSize(const std::string& aString) const
	{
		return myText->GetTextSize(aString);
	}

	const CU::Vector2<float>& TextInstance::GetCharSize() const
	{
		return myText->GetCharSize();
	}
}