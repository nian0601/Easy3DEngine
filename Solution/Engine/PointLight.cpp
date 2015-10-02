#include "stdafx.h"

#include "Camera.h"
#include "Instance.h"
#include "ModelLoader.h"
#include "Model.h"
#include "PointLight.h"

namespace Easy3D
{

	void PointLight::Initiate()
	{
		ModelProxy* model = Engine::GetInstance()->GetModelLoader()->LoadLightCube(1, 1, 1);
		myInstance = new Instance(*model);
	}

	void PointLight::Render(Camera* aCamera)
	{
		if (this != nullptr)
			myInstance->Render(*aCamera);
	}

};