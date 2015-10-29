#pragma once
#include <unordered_map>

namespace Easy3D
{
	class Font;
	class FontContainer
	{
	public:
		~FontContainer();

		Font* GetFont(const std::string& aPath);

	private:
		void LoadFont(const std::string& aPath);

		std::unordered_map<std::string, Font*> myFonts;
	};
}