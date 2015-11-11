#pragma once
#include <unordered_map>

namespace Easy3D
{
	class Font;
	class FontContainer
	{
	public:
		static void Create();
		static void Destroy();
		static FontContainer* GetInstance();

		Font* GetFont(const std::string& aPath);

	private:
		~FontContainer();

		void LoadFont(const std::string& aPath);

		std::unordered_map<std::string, Font*> myFonts;

		static FontContainer* myInstance;
	};
}