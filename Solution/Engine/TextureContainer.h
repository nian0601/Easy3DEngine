#pragma once
#include <unordered_map>
namespace Easy3D
{
	class Texture;

	class TextureContainer
	{
	public:
		static void Create();
		static void Destroy();
		static TextureContainer* GetInstance();
		
		Texture* GetTexture(const std::string& aFileName);

	private:
		~TextureContainer();

		void LoadTexture(const std::string& aFileName);

		std::unordered_map<std::string, Texture*> myTextures;

		static TextureContainer* myInstance;
	};
}