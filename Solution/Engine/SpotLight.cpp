#include "stdafx.h"

#include "Model.h"
#include "ModelLoader.h"
#include "Instance.h"
#include "SpotLight.h"


namespace Easy3D
{

	void SpotLight::Init()
	{
		ModelProxy* model = Engine::GetInstance()->GetModelLoader()->LoadLightCube(1, 1, 1);
		myInstance = new Instance(*model);
	}

	void SpotLight::Render(Camera* aCamera)
	{
		if (myInstance != nullptr)
		{
			myInstance->Render(*aCamera);
		}
	}

};