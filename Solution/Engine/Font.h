#pragma once
#include <Vector.h>

namespace Easy3D
{
	class Texture;

	class Font
	{
	public:
		struct CharacterData
		{
			CU::Vector2<float> myTopLeftUV;
			CU::Vector2<float> myBottomRightUV;
		};

		Font();

		bool Init(const char* aFontPath);
		CharacterData GetCharData(char aChar);

		const CU::Vector2<float>& GetCharSize() const;
		Texture* GetTexture();
	private:


		CU::Vector2<float> myCharSize;
		Texture* myTexture;
	};
}

inline const CU::Vector2<float>& Easy3D::Font::GetCharSize() const
{
	return myCharSize;
}

inline Easy3D::Texture* Easy3D::Font::GetTexture()
{
	return myTexture;
}