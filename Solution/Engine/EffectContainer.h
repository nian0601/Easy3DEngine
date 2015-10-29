#pragma once
#include <string>
#include <unordered_map>

namespace Easy3D
{
	class BaseEffect;
	class Effect2D;
	class Effect3D;
	class ParticleEffect;

	class EffectContainer
	{
	public:
		~EffectContainer();

		BaseEffect* GetBaseEffect(const std::string& aFilePath);
		Effect2D* Get2DEffect(const std::string& aFilePath);
		Effect3D* Get3DEffect(const std::string& aFilePath);
		ParticleEffect* GetParticleEffect(const std::string& aFilePath);

		void SetCubeMap(const std::string& aFilePath);	
	private:
		void LoadBaseEffect(const std::string& aFilePath);
		void ReloadBaseEffect(const std::string& aFilePath);

		void Load2DEffect(const std::string& aFilePath);
		void Reload2DEffect(const std::string& aFilePath);

		void Load3DEffect(const std::string& aFilePath);
		void Reload3DEffect(const std::string& aFilePath);

		void LoadParticleEffect(const std::string& aFilePath);
		void ReloadParticleEffect(const std::string& aFilePath);

		std::unordered_map<std::string, BaseEffect*> myBaseEffects;
		std::unordered_map<std::string, Effect2D*> my2DEffects;
		std::unordered_map<std::string, Effect3D*> my3DEffects;
		std::unordered_map<std::string, ParticleEffect*> myParticleEffects;
		std::string myCubeMap;
	};
}