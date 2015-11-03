#include "stdafx.h"

#include "Model.h"
#include "ModelProxy.h"

namespace Easy3D
{
	ModelProxy::ModelProxy()
		: myModel(nullptr)
	{
	}

	void ModelProxy::Render(const CU::Matrix44<float>& aOrientation)
	{
		if (myModel != nullptr)
		{
			myModel->Render(aOrientation);
		}
	}

	void ModelProxy::SetModel(Model* aModel)
	{
		myModel = aModel;
	}

	Model* ModelProxy::GetModel()
	{
		return myModel;
	}

	Effect3D* ModelProxy::GetEffect()
	{
		if (myModel == nullptr)
		{
			return nullptr;
		}

		return myModel->GetEffect();
	}

	void ModelProxy::SetEffect(Effect3D* aEffect)
	{
		myModel->SetEffect(aEffect);
	}
}