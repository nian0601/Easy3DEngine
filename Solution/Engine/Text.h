#pragma once
#include "BaseModel.h"

namespace Easy3D
{
	class Font;

	class Text : public BaseModel
	{
	public:
		void Init(const std::string& aFontPath);

		void Render(const std::string& aString, const CU::Vector2<float>& aPosition
			, const CU::Vector2<float>& aScale = { 1.f, 1.f }
			, const CU::Vector4<float>& aColor = { 1.f, 1.f, 1.f, 1.f });

		float GetTextWidth() const;
		CU::Vector2<float> GetTextSize(const std::string& aString) const;
		const CU::Vector2<float>& GetCharSize() const;

	private:
		void ConstructBuffers(const std::string& aString);

		CU::GrowingArray<VertexPosUV> myVertices;
		CU::GrowingArray<int> myIndices;

		Font* myFont;

		CU::Vector2<float> myCharSize;
		float myTextWidth;
		float myCharSpacing;

		std::string myLastText;
	};

	inline const CU::Vector2<float>& Text::GetCharSize() const
	{
		return myCharSize;
	}
}                                                   