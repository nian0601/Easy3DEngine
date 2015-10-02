#pragma once
#include <unordered_map>

namespace Prism
{
	class Font;
	class FontContainer
	{
	public:
		Font* GetFont(const std::string& aPath);

	private:
		void LoadFont(const std::string& aPath);

		std::unordered_map<std::string, Font*> myFonts;
	};
}