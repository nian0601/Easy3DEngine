#pragma once

#include <unordered_map>

namespace Easy3D
{
	class ParticleEmitterData;
	class ParticleEmitterInstance;

	class EmitterContainer
	{
	public:
		~EmitterContainer();

		ParticleEmitterInstance* CreateEmitter(const std::string& aFile);

	private:
		void LoadData(const std::string& aFile);

		std::unordered_map<std::string, ParticleEmitterData*> myEmitterDatas;
	};
}

