#pragma once
#include "Base2DModel.h"

namespace Easy3D
{
	class Font;

	class Text : public Base2DModel
	{
	public:
		void Init(const std::string& aFontPath);

		void Render(const std::string& aString, const float aDrawX, const float aDrawY
			, const CU::Vector4<float>& aColor = CU::Vector4<float>(1.f, 1.f, 1.f, 1.f), const float aScale = 1.f);

		float GetTextWidth() const;
		CU::Vector2<float> GetTextSize(const char* aString) const;

	private:
		void UpdateSentence(const std::string& aString, const float aDrawX, const float aDrawY,
			const CU::Vector4<float>& aColor, const float aScale);

		CU::GrowingArray<VertexPosColorUV> myVertices;
		CU::GrowingArray<int> myIndices;

		Font* myFont;

		bool myHasText;
		CU::Vector2<float> myCharSize;
		float myTextWidth;

		std::string myLastText;
		float myLastDrawX;
		float myLastDrawY;
		float myLastScale;

		float myCharSpacing;
	};
}                                                   