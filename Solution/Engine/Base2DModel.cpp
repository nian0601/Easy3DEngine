#include "stdafx.h"

#include "Base2DModel.h"
#include <D3D11.h>
#include <d3dx11effect.h>
#include "Effect2D.h"
#include "Effect3D.h"
#include "IndexBufferWrapper.h"
#include "Surface.h"


namespace Easy3D
{

	Base2DModel::Base2DModel()
	{
		myVertexBufferDesc = new D3D11_BUFFER_DESC();
		myIndexBufferDesc = new D3D11_BUFFER_DESC();
		myInitData = new D3D11_SUBRESOURCE_DATA();

		myEffect = nullptr;
	}

	Base2DModel::~Base2DModel()
	{
		if (myVertexBuffer->myVertexBuffer != nullptr)
		{
			myVertexBuffer->myVertexBuffer->Release();
			delete myVertexBuffer;
		}

		if (myIndexBuffer->myIndexBuffer != nullptr)
		{
			myIndexBuffer->myIndexBuffer->Release();
			delete myIndexBuffer;
		}
		
		delete myVertexBufferDesc;
		delete myIndexBufferDesc;
		delete myInitData;
		delete mySurface;
	}

	void Base2DModel::Render(const CU::Vector2<float>& aPosition, const CU::Vector2<float>& aScale 
		, const CU::Vector4<float>& aColor)
	{
		Engine::GetInstance()->DisableZBuffer();

		myPosition = aPosition;
		myScale = aScale;

		float blendFactor[4];
		blendFactor[0] = 0.f;
		blendFactor[1] = 0.f;
		blendFactor[2] = 0.f;
		blendFactor[3] = 0.f;

		myEffect->SetBlendState(myBlendState, blendFactor);
		myEffect->SetProjectionMatrix(Engine::GetInstance()->GetOrthogonalMatrix());
		myEffect->UpdatePosAndScale(aPosition, aScale);
		myEffect->SetPosAndScale();
		myEffect->SetColor(aColor);

		Engine::GetInstance()->GetContex()->IASetInputLayout(myVertexLayout);
		Engine::GetInstance()->GetContex()->IASetVertexBuffers(myVertexBuffer->myStartSlot
			, myVertexBuffer->myNumberOfBuffers, &myVertexBuffer->myVertexBuffer
			, &myVertexBuffer->myStride, &myVertexBuffer->myByteOffset);
		Engine::GetInstance()->GetContex()->IASetIndexBuffer(myIndexBuffer->myIndexBuffer
			, myIndexBuffer->myIndexBufferFormat, myIndexBuffer->myByteOffset);


		D3DX11_TECHNIQUE_DESC techDesc;
		myEffect->GetTechnique()->GetDesc(&techDesc);

		mySurface->Activate();

		for (UINT i = 0; i < techDesc.Passes; ++i)
		{
			myEffect->GetTechnique()->GetPassByIndex(i)->Apply(0, Engine::GetInstance()->GetContex());
			Engine::GetInstance()->GetContex()->DrawIndexed(mySurface->GetIndexCount(), mySurface->GetVertexStart(), 0);
		}

		Engine::GetInstance()->EnableZBuffer();
	}


	void Base2DModel::InitIndexBuffer()
	{
		myIndexBuffer = new IndexBufferWrapper();
		myIndexBuffer->myIndexBufferFormat = DXGI_FORMAT_R32_UINT;
		myIndexBuffer->myByteOffset = 0;


		ZeroMemory(myIndexBufferDesc, sizeof(myIndexBufferDesc));
		myIndexBufferDesc->Usage = D3D11_USAGE_IMMUTABLE;
		myIndexBufferDesc->BindFlags = D3D11_BIND_INDEX_BUFFER;
		myIndexBufferDesc->CPUAccessFlags = 0;
		myIndexBufferDesc->MiscFlags = 0;
		myIndexBufferDesc->StructureByteStride = 0;
	}

	void Base2DModel::InitSurface(const std::string& aResourceName, const std::string& aFileName)
	{
		mySurface = new Surface();

		mySurface->SetEffect(myEffect);
		mySurface->SetIndexCount(0);
		mySurface->SetIndexStart(0);
		mySurface->SetVertexCount(0);
		mySurface->SetVertexStart(0);
		mySurface->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		mySurface->SetTexture(aResourceName, aFileName, true);
	}

	void Base2DModel::InitBlendState()
	{
		D3D11_BLEND_DESC blendDesc;
		blendDesc.AlphaToCoverageEnable = true;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		HRESULT hr = Engine::GetInstance()->GetDevice()->CreateBlendState(&blendDesc, &myBlendState);
		if (FAILED(hr) != S_OK)
		{
			DL_MESSAGE_BOX("Failed to CreateBlendState", "Text::InitBlendState", MB_ICONWARNING);
		}
	}

	void Base2DModel::SetupVertexBuffer(int aVertexCount, size_t aVertexSize, char* aVertexData)
	{
		TIME_FUNCTION;


		if (myVertexBuffer->myVertexBuffer != nullptr)
			myVertexBuffer->myVertexBuffer->Release();

		myVertexBufferDesc->ByteWidth = aVertexSize * aVertexCount;
		myInitData->pSysMem = aVertexData;


		HRESULT hr = Engine::GetInstance()->GetDevice()->CreateBuffer(myVertexBufferDesc, myInitData, &myVertexBuffer->myVertexBuffer);
		if (FAILED(hr) != S_OK)
		{
			DL_MESSAGE_BOX("Failed to SetupVertexBuffer", "Text::SetupVertexBuffer", MB_ICONWARNING);
		}
	}

	void Base2DModel::SetupIndexBuffer(int aIndexCount, char* aIndexData)
	{
		TIME_FUNCTION;

		if (myIndexBuffer->myIndexBuffer != nullptr)
			myIndexBuffer->myIndexBuffer->Release();

		myIndexBufferDesc->ByteWidth = sizeof(UINT) * aIndexCount;
		myInitData->pSysMem = aIndexData;


		HRESULT hr = Engine::GetInstance()->GetDevice()->CreateBuffer(myIndexBufferDesc, myInitData,
			&myIndexBuffer->myIndexBuffer);
		if (FAILED(hr) != S_OK)
		{
			DL_MESSAGE_BOX("Failed to SetupIndexBuffer", "Text::SetupIndexBuffer", MB_ICONWARNING);
		}
	}

	void Base2DModel::OnEffectLoad()
	{
		mySurface->ReloadSurface();
	}
}