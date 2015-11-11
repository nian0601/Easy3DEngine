#pragma once

#include <unordered_map>

namespace Easy3D
{
	class ParticleEmitterData;
	class ParticleEmitterInstance;

	class EmitterContainer
	{
	public:
		static void Create();
		static void Destroy();
		static EmitterContainer* GetInstance();

		ParticleEmitterInstance* CreateEmitter(const std::string& aFile);

	private:
		~EmitterContainer();

		void LoadData(const std::string& aFile);

		std::unordered_map<std::string, ParticleEmitterData*> myEmitterDatas;

		static EmitterContainer* myInstance;
	};
}

