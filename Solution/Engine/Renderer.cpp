#include "stdafx.h"

#include <d3dx11effect.h>
#include "FullScreenHelper.h"
#include "Renderer.h"
#include "Scene.h"
#include "Structs.h"
#include "Texture.h"


namespace Easy3D
{
	Renderer::Renderer()
	{
		mySceneIndex = 0;
		for (int i = 0; i < MAX_SCENE_COUNT; ++i)
		{
			myScenes[i] = new SceneData();
			myScenes[i]->myScene = new Texture();
			myScenes[i]->myScene->Init(static_cast<float>(Engine::GetInstance()->GetWindowSize().x)
				, static_cast<float>(Engine::GetInstance()->GetWindowSize().y)
				, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
				, DXGI_FORMAT_R8G8B8A8_UNORM);

			myScenes[i]->myFinished = new Texture();
			myScenes[i]->myFinished->Init(static_cast<float>(Engine::GetInstance()->GetWindowSize().x)
				, static_cast<float>(Engine::GetInstance()->GetWindowSize().y)
				, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
				, DXGI_FORMAT_R8G8B8A8_UNORM);

			myScenes[i]->myVelocity = new Texture();
			myScenes[i]->myVelocity->Init(static_cast<float>(Engine::GetInstance()->GetWindowSize().x)
				, static_cast<float>(Engine::GetInstance()->GetWindowSize().y)
				, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
				, DXGI_FORMAT_R16G16_FLOAT);
		}

		myCombinedScenes = new Texture();
		myCombinedScenes->Init(static_cast<float>(Engine::GetInstance()->GetWindowSize().x)
			, static_cast<float>(Engine::GetInstance()->GetWindowSize().y)
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
			, DXGI_FORMAT_R8G8B8A8_UNORM);

		myFontTexture = new Texture();
		myFontTexture->Init(static_cast<float>(Engine::GetInstance()->GetWindowSize().x)
			, static_cast<float>(Engine::GetInstance()->GetWindowSize().y)
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
			, DXGI_FORMAT_R8G8B8A8_UNORM);

		myFinalTexture = new Texture();
		myFinalTexture->Init(static_cast<float>(Engine::GetInstance()->GetWindowSize().x)
			, static_cast<float>(Engine::GetInstance()->GetWindowSize().y)
			, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
			, DXGI_FORMAT_R8G8B8A8_UNORM);

		myFullScreenHelper = new FullScreenHelper();
	}
	
	Renderer::~Renderer()
	{
		for (int i = 0; i < MAX_SCENE_COUNT; ++i)
		{
			delete myScenes[i]->myScene;
			myScenes[i]->myScene = nullptr;

			delete myScenes[i]->myFinished;
			myScenes[i]->myFinished = nullptr;

			delete myScenes[i]->myVelocity;
			myScenes[i]->myVelocity = nullptr;
		}

		delete myCombinedScenes;
		delete myFontTexture;
		delete myFinalTexture;

		delete myFullScreenHelper;
	}

	void Easy3D::Renderer::ProcessScene(Scene* aScene, int aEffect)
	{
		DL_ASSERT_EXP(mySceneIndex < MAX_SCENE_COUNT, "[Renderer]: Tried to render too many scenes");

		SceneData* currData = myScenes[mySceneIndex];

		float clearcolor[4] = { 0.3f, 0.3f, 0.3f, 1 };
		Engine::GetInstance()->GetContex()->ClearRenderTargetView(currData->myScene->GetRenderTargetView(), clearcolor);
		Engine::GetInstance()->GetContex()->ClearRenderTargetView(currData->myFinished->GetRenderTargetView(), clearcolor);
		float velClearcolor[4] = { 0.f, 0.f, 0.f, 1 };
		Engine::GetInstance()->GetContex()->ClearRenderTargetView(currData->myVelocity->GetRenderTargetView(), velClearcolor);

		//ID3D11RenderTargetView* renderTarget[2] = {
		//	currData->myScene->GetRenderTargetView(),
		//	currData->myVelocity->GetRenderTargetView()
		//};
		//
		//Engine::GetInstance()->GetContex()->OMSetRenderTargets(2, &renderTarget[0]
		//	, Engine::GetInstance()->GetDepthStencilView());

		ID3D11RenderTargetView* target = currData->myScene->GetRenderTargetView();
		Engine::GetInstance()->GetContex()->OMSetRenderTargets(1, &target
			, Engine::GetInstance()->GetDepthStencilView());
		aScene->Render();


		Engine::GetInstance()->SetDepthBufferState(eDepthStencil::Z_DISABLED);
		myFullScreenHelper->Process(currData, aEffect);
		Engine::GetInstance()->SetDepthBufferState(eDepthStencil::Z_ENABLED);

		++mySceneIndex;
	}
	
	void Easy3D::Renderer::FinalRender()
	{
		Engine::GetInstance()->RestoreViewPort();
		myFullScreenHelper->ActivateBuffers();

		float clearcolor[4] = { 0.3f, 0.3f, 0.3f, 1 };
		Engine::GetInstance()->GetContex()->ClearRenderTargetView(myCombinedScenes->GetRenderTargetView(), clearcolor);
		Engine::GetInstance()->GetContex()->ClearRenderTargetView(myFinalTexture->GetRenderTargetView(), clearcolor);

		if (mySceneIndex == 1)
		{
			myFullScreenHelper->Combine(myScenes[0]->myFinished, myCombinedScenes);
		}
		else if (mySceneIndex > 1)
		{
			myFullScreenHelper->Combine(myScenes[0]->myFinished, myScenes[1]->myFinished, myCombinedScenes);
		
			for (int i = 2; i < mySceneIndex; ++i)
			{
				myFullScreenHelper->Combine(myScenes[i]->myFinished, myCombinedScenes);
			}
		}


		Engine::GetInstance()->SetDepthBufferState(eDepthStencil::Z_DISABLED);
		myFullScreenHelper->Combine(myCombinedScenes, myFontTexture, myFinalTexture);
		myFullScreenHelper->RenderToScreen(myFinalTexture);
		Engine::GetInstance()->SetDepthBufferState(eDepthStencil::Z_ENABLED);

		mySceneIndex = 0;
	}

	void Renderer::StartFontRendering()
	{
		float clearcolor[4] = { 0.3f, 0.3f, 0.3f, 1 };
		Engine::GetInstance()->GetContex()->ClearRenderTargetView(myFontTexture->GetRenderTargetView(), clearcolor);

		ID3D11RenderTargetView* target = myFontTexture->GetRenderTargetView();
		Engine::GetInstance()->GetContex()->OMSetRenderTargets(1, &target
			, Engine::GetInstance()->GetDepthStencilView());
	}

	void Renderer::EndFontRendering()
	{

	}
}
