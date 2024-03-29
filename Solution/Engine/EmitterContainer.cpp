#include "stdafx.h"

#include "EmitterContainer.h"
#include "ParticleEmitterData.h"
#include "ParticleEmitterInstance.h"

namespace Easy3D
{
	EmitterContainer* EmitterContainer::myInstance = nullptr;

	void EmitterContainer::Create()
	{
		myInstance = new EmitterContainer();
	}

	void EmitterContainer::Destroy()
	{
		delete myInstance;
	}

	EmitterContainer* EmitterContainer::GetInstance()
	{
		DL_ASSERT_EXP(myInstance != nullptr, "EmitterContainer: myInstance is nullptr, forgot to create?");
		return myInstance;
	}

	EmitterContainer::~EmitterContainer()
	{
		for (auto it = myEmitterDatas.begin(); it != myEmitterDatas.end(); ++it)
		{
			delete it->second;
			it->second = nullptr;
		}
	}

	ParticleEmitterInstance* EmitterContainer::CreateEmitter(const std::string& aFile)
	{
		if (myEmitterDatas.find(aFile) == myEmitterDatas.end())
		{
			LoadData(aFile);
		}

		ParticleEmitterInstance* newEmitter = new ParticleEmitterInstance(*myEmitterDatas[aFile]);
		return newEmitter;
	}

	void EmitterContainer::LoadData(const std::string& aFile)
	{
		Easy3D::ParticleEmitterData* particleData = new Easy3D::ParticleEmitterData();
		particleData->Init(aFile);

		myEmitterDatas[aFile] = particleData;
	}
}