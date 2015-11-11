#pragma once

namespace Easy3D
{
	class Text;

	class TextInstance
	{
	public:
		TextInstance();
		~TextInstance();

		void Init(const std::string& aFontPath);

		void Render(const std::string& aString, const CU::Vector2<float>& aPosition
			, const CU::Vector2<float>& aScale = { 1.f, 1.f }
		, const CU::Vector4<float>& aColor = { 1.f, 1.f, 1.f, 1.f });

		float GetTextWidth() const;
		CU::Vector2<float> GetTextSize(const std::string& aString) const;
		const CU::Vector2<float>& GetCharSize() const;

	private:
		Text* myText;
	};
}