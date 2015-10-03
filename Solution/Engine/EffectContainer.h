#pragma once
#include <string>
#include <unordered_map>

namespace Easy3D
{
	class Effect3D;

	class EffectContainer
	{
	public:
		Effect3D* Get3DEffect(const std::string& aFilePath);

		void SetCubeMap(const std::string& aFilePath);	
	private:
		void Load3DEffect(const std::string& aFilePath);
		void Reload3DEffect(const std::string& aFilePath);

		std::unordered_map<std::string, Effect3D*> my3DEffects;
		std::string myCubeMap;
	};
}