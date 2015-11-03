#include "stdafx.h"

#include "Camera.h"
#include <d3dx11effect.h>
#include "Effect3D.h"
#include "RenderProcessTarget.h"

namespace Easy3D
{
	RenderProcessTarget::RenderProcessTarget()
		: myCamera(nullptr)
	{

	}

	void RenderProcessTarget::SetCamera(Camera* aCamera)
	{
		myCamera = aCamera;
	}

	void RenderProcessTarget::AddModel(Model* aModel, const CU::Matrix44<float>& aWorldMatrix)
	{
		if (myRenderInfo.find(aModel) == myRenderInfo.end())
		{
			myRenderInfo[aModel].Init(8);
		}

		myRenderInfo[aModel].Add(aWorldMatrix);
	}

	void RenderProcessTarget::RenderNormal()
	{
		DL_ASSERT_EXP(myCamera != nullptr, "Tried to render without a camera");

		//Sleep(5);

		Effect3D* oldEffect = nullptr;

		//For each model
		for (auto it = myRenderInfo.begin(); it != myRenderInfo.end(); ++it)
		{
			Model* currModel = it->first;
			CU::GrowingArray<CU::Matrix44<float>>& matrices = it->second;
			Effect3D* currEffect = currModel->GetEffect();

			if (currEffect != oldEffect)
			{
				oldEffect = currEffect;
				currEffect->SetViewMatrix(CU::InverseSimple(myCamera->GetOrientation()));
				currEffect->SetProjectionMatrix(myCamera->GetProjection());
				currEffect->SetScaleVector({ 1.f, 1.f, 1.f });
			}
			
			currModel->SetGPUState();

			D3DX11_TECHNIQUE_DESC techDesc;
			ID3DX11EffectTechnique* tech = currEffect->GetEffect()->GetTechniqueByName("Render");
			tech->GetDesc(&techDesc);

			//For each instance
			for (int i = 0; i < matrices.Size(); ++i)
			{
				currEffect->SetWorldMatrix(matrices[i]);

				for (UINT j = 0; j < techDesc.Passes; ++j)
				{
					tech->GetPassByIndex(j)->Apply(0
						, Engine::GetInstance()->GetContex());
					
					Engine::GetInstance()->GetContex()->DrawIndexed(currModel->GetIndexCount(0)
						, currModel->GetVertexStart(0), 0);
				}
			}
			
			matrices.RemoveAll();
		}
	}

	void RenderProcessTarget::RenderInstanced()
	{
		DL_ASSERT_EXP(myCamera != nullptr, "Tried to render without a camera");

		//Sleep(5);

		Effect3D* oldEffect = nullptr;

		//For each model
		for (auto it = myRenderInfo.begin(); it != myRenderInfo.end(); ++it)
		{
			Model* currModel = it->first;
			CU::GrowingArray<CU::Matrix44<float>>& matrices = it->second;
			Effect3D* currEffect = currModel->GetEffect();

			if (currEffect != oldEffect)
			{
				oldEffect = currEffect;
				currEffect->SetViewMatrix(CU::InverseSimple(myCamera->GetOrientation()));
				currEffect->SetProjectionMatrix(myCamera->GetProjection());
				currEffect->SetScaleVector({ 1.f, 1.f, 1.f });
			}

			currModel->SetGPUState(matrices);

			D3DX11_TECHNIQUE_DESC techDesc;
			ID3DX11EffectTechnique* tech = currEffect->GetEffect()->GetTechniqueByName("Render");
			tech->GetDesc(&techDesc);

			for (UINT j = 0; j < techDesc.Passes; ++j)
			{
				tech->GetPassByIndex(j)->Apply(0
					, Engine::GetInstance()->GetContex());

				Engine::GetInstance()->GetContex()->DrawIndexedInstanced(currModel->GetIndexCount(0), matrices.Size()
					, 0, currModel->GetVertexStart(0), 0);
			}

			matrices.RemoveAll();
		}
	}
}