#include "stdafx.h"

#include "BaseEffect.h"
#include <d3dx11effect.h>
#include "FullScreenHelper.h"
#include "IndexBufferWrapper.h"
#include <Macros.h>
#include "VertexBufferWrapper.h"
#include "Structs.h"


namespace Easy3D
{
	FullScreenHelper::FullScreenHelper()
	{
		myClearColor[0] = 0.3f;
		myClearColor[1] = 0.3f;
		myClearColor[2] = 1.f;
		myClearColor[3] = 1.f;

		CreateCombineData();
		CreateRenderToTextureData();
		CreateDownSampleData();
		CreateHDRData();
		CreateBloomData();

		myProcessedTexture = new Texture();
		myProcessedTexture->Init(static_cast<float>(Engine::GetInstance()->GetWindowSize().x)
			, static_cast<float>(Engine::GetInstance()->GetWindowSize().y)
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
			, DXGI_FORMAT_R8G8B8A8_UNORM);

		D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		D3DX11_PASS_DESC passDesc;
		myRenderToTextureData.myEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);

		ID3D11InputLayout* input = nullptr;
		HRESULT hr = Engine::GetInstance()->GetDevice()->CreateInputLayout(vertexDesc
			, ARRAYSIZE(vertexDesc), passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &input);
		if (FAILED(hr) != S_OK)
		{
			DL_MESSAGE_BOX("Failed to CreateInputLayout", "Model2D::Init", MB_ICONWARNING);
		}

		myVertexLayout.Set(input);

		InitVertexBuffer(sizeof(VertexPosUV), D3D11_USAGE_IMMUTABLE, 0);
		InitIndexBuffer();

		CreateVertices();

		ZeroMemory(&myViewPort, sizeof(D3D11_VIEWPORT));

		myViewPort.TopLeftX = 0;
		myViewPort.TopLeftY = 0;
		myViewPort.Width = static_cast<FLOAT>(Engine::GetInstance()->GetWindowSize().x / 2);
		myViewPort.Height = static_cast<FLOAT>(Engine::GetInstance()->GetWindowSize().y / 2);
		myViewPort.MinDepth = 0.f;
		myViewPort.MaxDepth = 1.f;
	}
	
	FullScreenHelper::~FullScreenHelper()
	{
		delete myBloomData.myMiddleMan;
		myBloomData.myMiddleMan = nullptr;
		delete myBloomData.myFinalTexture;
		myBloomData.myFinalTexture = nullptr;

		delete myDownSampleData.myBloomDownSample[0];
		myDownSampleData.myBloomDownSample[0] = nullptr;
		delete myDownSampleData.myBloomDownSample[1];
		myDownSampleData.myBloomDownSample[1] = nullptr;


		myDownSampleData.myHDRDownSamples.DeleteAll();

		delete myProcessedTexture;
		myProcessedTexture = nullptr;
	}

	void FullScreenHelper::Process(SceneData* aSceneData, int aEffect)
	{
		Engine::GetInstance()->RestoreViewPort();
		ActivateBuffers();

		Engine::GetInstance()->GetContex()->ClearRenderTargetView(myProcessedTexture->GetRenderTargetView(), myClearColor);
		Combine(aSceneData->myScene, myProcessedTexture);

		if (aEffect & ePostProcess::HDR)
		{
			HDRDownSample(aSceneData->myScene);
			HDREffect(aSceneData->myScene, aSceneData->myFinished);
		}

		//if (aEffect & ePostProcess::MOTION_BLUR)
		//{
		//	MotionBlurEffect(myPostProcessTexture->GetShaderView()
		//		, someRendererTextures[eRendererTextures::VELOCITY_TEXTURE]->GetShaderView());
		//}


		if (aEffect & ePostProcess::BLOOM)
		{
			BloomEffect(myProcessedTexture);
			Combine(myProcessedTexture, myBloomData.myFinalTexture);
		}

		Combine(myProcessedTexture, aSceneData->myFinished);
	}

	void FullScreenHelper::Combine(Texture* aSource, Texture* aTarget)
	{
		DL_ASSERT_EXP(aSource != aTarget, "[Combine]: Cant use Texture as both Source and Target");

		myRenderToTextureData.mySource->SetResource(aSource->GetShaderView());

		ID3D11RenderTargetView* target = aTarget->GetRenderTargetView();
		Engine::GetInstance()->GetContex()->OMSetRenderTargets(1, &target
			, Engine::GetInstance()->GetDepthStencilView());

		Render(myRenderToTextureData.myEffect);

		myRenderToTextureData.mySource->SetResource(NULL);
	}

	void FullScreenHelper::Combine(Texture* aSourceA, Texture* aSourceB, Texture* aTarget)
	{
		DL_ASSERT_EXP(aSourceA != aTarget, "[Combine]: Cant use Texture as both Source and Target");
		DL_ASSERT_EXP(aSourceB != aTarget, "[Combine]: Cant use Texture as both Source and Target");

		myCombineData.mySourceA->SetResource(aSourceA->GetShaderView());
		myCombineData.mySourceB->SetResource(aSourceB->GetShaderView());

		ID3D11RenderTargetView* target = aTarget->GetRenderTargetView();
		Engine::GetInstance()->GetContex()->OMSetRenderTargets(1, &target
			, Engine::GetInstance()->GetDepthStencilView());

		Render(myCombineData.myEffect);

		myCombineData.mySourceA->SetResource(NULL);
		myCombineData.mySourceB->SetResource(NULL);
	}

	void FullScreenHelper::RenderToScreen(Texture* aToBackbufferTexture)
	{
		Engine::GetInstance()->RestoreViewPort();

		ID3D11RenderTargetView* backbuffer = Engine::GetInstance()->GetBackbuffer();
		Engine::GetInstance()->GetContex()->ClearRenderTargetView(backbuffer, myClearColor);
		Engine::GetInstance()->GetContex()->OMSetRenderTargets(1, &backbuffer
			, Engine::GetInstance()->GetDepthStencilView());

		myRenderToTextureData.mySource->SetResource(aToBackbufferTexture->GetShaderView());

		Render(myRenderToTextureData.myEffect);

		myRenderToTextureData.mySource->SetResource(NULL);
	}

	void FullScreenHelper::ActivateBuffers()
	{
		ID3D11Buffer* buf = myVertexBuffer->myVertexBuffer.Get();
		Engine::GetInstance()->GetContex()->IASetInputLayout(myVertexLayout.Get());
		Engine::GetInstance()->GetContex()->IASetVertexBuffers(myVertexBuffer->myStartSlot,
			myVertexBuffer->myNumberOfBuffers, &buf,
			&myVertexBuffer->myStride, &myVertexBuffer->myByteOffset);
		Engine::GetInstance()->GetContex()->IASetIndexBuffer(myIndexBuffer->myIndexBuffer.Get(),
			myIndexBuffer->myIndexBufferFormat, myIndexBuffer->myByteOffset);
		Engine::GetInstance()->GetContex()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	void FullScreenHelper::CreateVertices()
	{
		
		CU::GrowingArray<VertexPosUV> vertices;
		vertices.Init(4);
		vertices.Add({ { -1.f, -1.f, 0.f }, { 0.f, 1.f } }); //topleft
		vertices.Add({ { 1.f, -1.f, 0.f }, { 1.f, 1.f } }); //topright
		vertices.Add({ { -1.f, 1.f, 0.f }, { 0.f, 0.f } }); //bottomleft
		vertices.Add({ { 1.f, 1.f, 0.f }, { 1.f, 0.f } }); //bottomright

		CU::GrowingArray<int> indices;
		indices.Init(6);

		indices.Add(0);
		indices.Add(2);
		indices.Add(1);

		indices.Add(1);
		indices.Add(2);
		indices.Add(3);

		SetupVertexBuffer(vertices.Size(), sizeof(VertexPosUV), reinterpret_cast<char*>(&vertices[0]));
		SetupIndexBuffer(indices.Size(), reinterpret_cast<char*>(&indices[0]));
	}

	void FullScreenHelper::CreateCombineData()
	{
		myCombineData.myEffect = EffectContainer::GetInstance()->GetBaseEffect(
			"Data/Shader/SharedUtilities/Combine.fx");

		myCombineData.mySourceA 
			= myCombineData.myEffect->GetEffect()->GetVariableByName("SourceA")->AsShaderResource();
		myCombineData.mySourceB
			= myCombineData.myEffect->GetEffect()->GetVariableByName("SourceB")->AsShaderResource();
	}

	void FullScreenHelper::CreateRenderToTextureData()
	{
		myRenderToTextureData.myEffect = EffectContainer::GetInstance()->GetBaseEffect(
			"Data/Shader/SharedUtilities/RenderToTexture.fx");

		myRenderToTextureData.mySource
			= myRenderToTextureData.myEffect->GetEffect()->GetVariableByName("DiffuseTexture")->AsShaderResource();
	}

	void FullScreenHelper::CreateDownSampleData()
	{
		myDownSampleData.myEffect 
			= EffectContainer::GetInstance()->GetBaseEffect("Data/Shader/SharedUtilities/DownSample.fx");
		myDownSampleData.myTexture 
			= myDownSampleData.myEffect->GetEffect()->GetVariableByName("DiffuseTexture")->AsShaderResource();

		float width = static_cast<FLOAT>(Engine::GetInstance()->GetWindowSize().x);
		float height = static_cast<FLOAT>(Engine::GetInstance()->GetWindowSize().y);
		
		myDownSampleData.myHDRDownSamplesCount = static_cast<int>(HDRLog2(MIN(width, height)) + 1);
		myDownSampleData.myHDRDownSamples.Init(myDownSampleData.myHDRDownSamplesCount);
		float size = MIN(width, height);
		for (int i = 0; i < myDownSampleData.myHDRDownSamplesCount; ++i)
		{
			Texture* tex = new Texture();
			tex->Init(size, size, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL
				, DXGI_FORMAT_R8G8B8A8_UNORM);
			myDownSampleData.myHDRDownSamples.Add(tex);

			size /= 2.f;
		}

		myDownSampleData.myFinalHDRDownSample = myDownSampleData.myHDRDownSamples[myDownSampleData.myHDRDownSamplesCount - 1];
	}

	void FullScreenHelper::CreateHDRData()
	{
		myHDRData.myEffect
			= EffectContainer::GetInstance()->GetBaseEffect("Data/Shader/SharedUtilities/HDR.fx");

		myHDRData.myOriginalTexture 
			= myHDRData.myEffect->GetEffect()->GetVariableByName("Original")->AsShaderResource();
		myHDRData.myAverageColorTexture
			= myHDRData.myEffect->GetEffect()->GetVariableByName("AverageColor")->AsShaderResource();
	}

	void FullScreenHelper::CreateBloomData()
	{
		myBloomData.myFinalTexture = new Texture();
		myBloomData.myFinalTexture->Init(Engine::GetInstance()->GetWindowSize().x / 4.f
			, Engine::GetInstance()->GetWindowSize().y / 4.f
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
			, DXGI_FORMAT_R8G8B8A8_UNORM);

		myBloomData.myMiddleMan = new Texture();
		myBloomData.myMiddleMan->Init(Engine::GetInstance()->GetWindowSize().x / 4.f
			, Engine::GetInstance()->GetWindowSize().y / 4.f
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
			, DXGI_FORMAT_R8G8B8A8_UNORM);

		myBloomData.myEffect
			= EffectContainer::GetInstance()->GetBaseEffect("Data/Shader/SharedUtilities/Bloom.fx");
		myBloomData.myTexture
			= myBloomData.myEffect->GetEffect()->GetVariableByName("DiffuseTexture")->AsShaderResource();
		myBloomData.myTexelWidthVariable
			= myBloomData.myEffect->GetEffect()->GetVariableByName("TexelWidth")->AsScalar();
		myBloomData.myTexelHeightVariable
			= myBloomData.myEffect->GetEffect()->GetVariableByName("TexelHeight")->AsScalar();

		myBloomData.myTexelWidthVariable->SetFloat(1.f / (Engine::GetInstance()->GetWindowSize().x / 4));
		myBloomData.myTexelHeightVariable->SetFloat(1.f / (Engine::GetInstance()->GetWindowSize().y / 4));


		myDownSampleData.myBloomDownSample[0] = new Texture();
		myDownSampleData.myBloomDownSample[0]->Init(Engine::GetInstance()->GetWindowSize().x / 2.f
			, Engine::GetInstance()->GetWindowSize().y / 2.f
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
			, DXGI_FORMAT_R8G8B8A8_UNORM);

		myDownSampleData.myBloomDownSample[1] = new Texture();
		myDownSampleData.myBloomDownSample[1]->Init(Engine::GetInstance()->GetWindowSize().x / 4.f
			, Engine::GetInstance()->GetWindowSize().y / 4.f
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
			, DXGI_FORMAT_R8G8B8A8_UNORM);
	}

	float FullScreenHelper::HDRLog2(float aNumber)
	{
		return log(aNumber) / log(2.f);
	}

	void FullScreenHelper::Render(BaseEffect* aEffect)
	{
		D3DX11_TECHNIQUE_DESC techDesc;
		aEffect->GetTechnique()->GetDesc(&techDesc);
		for (UINT i = 0; i < techDesc.Passes; ++i)
		{
			aEffect->GetTechnique()->GetPassByIndex(i)->Apply(0, Engine::GetInstance()->GetContex());
			Engine::GetInstance()->GetContex()->DrawIndexed(6, 0, 0);
		}
	}

	void FullScreenHelper::Render(BaseEffect* aEffect, const std::string& aTechnique)
	{
		ID3DX11EffectTechnique* tech = aEffect->GetEffect()->GetTechniqueByName(aTechnique.c_str());
		DL_ASSERT_EXP(tech->IsValid() != 0, "[FullScreenHelper]: Tried to use invalid EffectTechnique");

		D3DX11_TECHNIQUE_DESC techDesc;
		tech->GetDesc(&techDesc);
		for (UINT i = 0; i < techDesc.Passes; ++i)
		{
			tech->GetPassByIndex(i)->Apply(0, Engine::GetInstance()->GetContex());
			Engine::GetInstance()->GetContex()->DrawIndexed(6, 0, 0);
		}
	}

	void FullScreenHelper::DownSample(Texture* aTarget, Texture* aSource, float aWidth, float aHeight, bool aHDRDownSample /*= false*/)
	{
		myViewPort.Width = aWidth;
		myViewPort.Height = aHeight;
		Engine::GetInstance()->GetContex()->RSSetViewports(1, &myViewPort);

		ID3D11RenderTargetView* target = aTarget->GetRenderTargetView();
		ID3D11DepthStencilView* depth = aTarget->GetDepthStencilView();
		Engine::GetInstance()->GetContex()->OMSetRenderTargets(1, &target, depth);
		float clearcolor[4] = { 0, 0, 0, 1 };
		Engine::GetInstance()->GetContex()->ClearRenderTargetView(target, clearcolor);

		myDownSampleData.myTexture->SetResource(aSource->GetShaderView());


		if (aHDRDownSample == true)
		{
			Render(myDownSampleData.myEffect, "HDR_DOWNSAMPLE");
		}
		else
		{
			Render(myDownSampleData.myEffect, "BLOOM_DOWNSAMPLE");
		}
	}

	void FullScreenHelper::HDRDownSample(Texture* aSource)
	{
		float width = static_cast<FLOAT>(Engine::GetInstance()->GetWindowSize().x);
		float height = static_cast<FLOAT>(Engine::GetInstance()->GetWindowSize().y);
		int hdrTextureCount = static_cast<int>(HDRLog2(MIN(width, height)) + 1);
		float size = MIN(width, height);

		for (int i = 0; i < hdrTextureCount; ++i)
		{
			if (i == 0)
			{
				DownSample(myDownSampleData.myHDRDownSamples[i], aSource, size, size, true);
			}
			else
			{
				DownSample(myDownSampleData.myHDRDownSamples[i]
					, myDownSampleData.myHDRDownSamples[i - 1], size, size, true);
			}

			size /= 2.f;
		}
	}

	void FullScreenHelper::HDREffect(Texture* aSource, Texture* aTarget)
	{
		Engine::GetInstance()->RestoreViewPort();

		ID3D11RenderTargetView* target = aTarget->GetRenderTargetView();
		Engine::GetInstance()->GetContex()->OMSetRenderTargets(1, &target
			, Engine::GetInstance()->GetDepthStencilView());

		myHDRData.myOriginalTexture->SetResource(aSource->GetShaderView());
		myHDRData.myAverageColorTexture->SetResource(myDownSampleData.myFinalHDRDownSample->GetShaderView());

		Render(myHDRData.myEffect);

		Engine::GetInstance()->GetContex()->ClearRenderTargetView(myProcessedTexture->GetRenderTargetView(), myClearColor);
		target = myProcessedTexture->GetRenderTargetView();
		Engine::GetInstance()->GetContex()->OMSetRenderTargets(1, &target
			, Engine::GetInstance()->GetDepthStencilView());

		Render(myHDRData.myEffect);
	}

	void FullScreenHelper::BloomEffect(Texture* aSource)
	{
		float width = static_cast<FLOAT>(Engine::GetInstance()->GetWindowSize().x / 2);
		float height = static_cast<FLOAT>(Engine::GetInstance()->GetWindowSize().y / 2);
		for (int i = 0; i < 2; ++i)
		{
			if (i == 0)
			{
				DownSample(myDownSampleData.myBloomDownSample[i], aSource, width, height);
			}
			else
			{
				DownSample(myDownSampleData.myBloomDownSample[i]
					, myDownSampleData.myBloomDownSample[i - 1], width, height);
			}

			width /= 2.f;
			height /= 2.f;

		}

		DoBloom(myDownSampleData.myBloomDownSample[1], myBloomData.myFinalTexture);
	}

	void FullScreenHelper::DoBloom(Texture* aSource, Texture* aTarget)
	{
		Engine::GetInstance()->RestoreViewPort();

		ID3D11RenderTargetView* target = myBloomData.myMiddleMan->GetRenderTargetView();
		Engine::GetInstance()->GetContex()->OMSetRenderTargets(1, &target
			, Engine::GetInstance()->GetDepthStencilView());
		myBloomData.myTexture->SetResource(aSource->GetShaderView());

		Render(myBloomData.myEffect, "BLOOM_X");

		target = aTarget->GetRenderTargetView();
		Engine::GetInstance()->GetContex()->OMSetRenderTargets(1, &target
			, Engine::GetInstance()->GetDepthStencilView());
		myBloomData.myTexture->SetResource(myBloomData.myMiddleMan->GetShaderView());

		Render(myBloomData.myEffect, "BLOOM_Y");
	}

}
