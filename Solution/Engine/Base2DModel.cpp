#include "stdafx.h"

#include "Effect3D.h"
#include "Base2DModel.h"
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

	void Base2DModel::InitSurface(const std::string& aFileName)
	{
		mySurface = new Surface();

		mySurface->SetEffect(myEffect);
		mySurface->SetIndexCount(0);
		mySurface->SetIndexStart(0);
		mySurface->SetVertexCount(0);
		mySurface->SetVertexStart(0);
		mySurface->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		mySurface->SetTexture("DiffuseTexture", aFileName, true);
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