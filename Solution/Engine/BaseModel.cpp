#include "stdafx.h"

#include "BaseModel.h"
#include <D3D11.h>
#include <d3dx11effect.h>
#include "Effect2D.h"
#include "Effect3D.h"
#include "IndexBufferWrapper.h"
#include "Surface.h"


namespace Easy3D
{

	BaseModel::BaseModel()
	{
		myVertexBufferDesc = new D3D11_BUFFER_DESC();
		myIndexBufferDesc = new D3D11_BUFFER_DESC();
		myInitData = new D3D11_SUBRESOURCE_DATA();

		myEffect = nullptr;

		mySurfaces.Init(1);

		myVertexBuffer = nullptr;
		myIndexBuffer = nullptr;
	}

	BaseModel::~BaseModel()
	{
		if (myVertexBuffer != nullptr && myVertexBuffer->myVertexBuffer != nullptr)
		{
			myVertexBuffer->myVertexBuffer->Release();
			delete myVertexBuffer;
		}

		if (myIndexBuffer != nullptr && myIndexBuffer->myIndexBuffer != nullptr)
		{
			myIndexBuffer->myIndexBuffer->Release();
			delete myIndexBuffer;
		}
		
		delete myVertexBufferDesc;
		delete myIndexBufferDesc;
		delete myInitData;

		if(myBlendState != nullptr)
		{
			myBlendState->Release();
		}

		if(myVertexLayout != nullptr)
		{
			myVertexLayout->Release();
		}
		mySurfaces.DeleteAll();
	}

	void BaseModel::Render()
	{
		Engine::GetInstance()->GetContex()->IASetInputLayout(myVertexLayout);
		Engine::GetInstance()->GetContex()->IASetVertexBuffers(myVertexBuffer->myStartSlot
			, myVertexBuffer->myNumberOfBuffers, &myVertexBuffer->myVertexBuffer
			, &myVertexBuffer->myStride, &myVertexBuffer->myByteOffset);
		Engine::GetInstance()->GetContex()->IASetIndexBuffer(myIndexBuffer->myIndexBuffer
			, myIndexBuffer->myIndexBufferFormat, myIndexBuffer->myByteOffset);


		D3DX11_TECHNIQUE_DESC techDesc;
		myEffect->GetTechnique()->GetDesc(&techDesc);

		for (int i = 0; i < mySurfaces.Size(); ++i)
		{
			mySurfaces[i]->Activate();

			for (UINT j = 0; j < techDesc.Passes; ++j)
			{
				myEffect->GetTechnique()->GetPassByIndex(j)->Apply(0, Engine::GetInstance()->GetContex());
				Engine::GetInstance()->GetContex()->DrawIndexed(mySurfaces[i]->GetIndexCount()
					, mySurfaces[i]->GetVertexStart(), 0);
			}
		}
	}

	void BaseModel::InitInputLayout(D3D11_INPUT_ELEMENT_DESC* aVertexDescArray, int aArraySize)
	{
		D3DX11_PASS_DESC passDesc;
		myEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);
		HRESULT hr = Engine::GetInstance()->GetDevice()->CreateInputLayout(aVertexDescArray
			, aArraySize, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &myVertexLayout);
		if (FAILED(hr) != S_OK)
		{
			DL_MESSAGE_BOX("Failed to CreateInputLayout", "Model2D::Init", MB_ICONWARNING);
		}

		Engine::GetInstance()->SetDebugName(myVertexLayout, "BaseModel::myVertexLayout");
	}

	void Easy3D::BaseModel::InitVertexBuffer(int aVertexSize, int aBufferUsage, int aCPUUsage)
	{
		myVertexBuffer = new VertexBufferWrapper();
		myVertexBuffer->myStride = aVertexSize;
		myVertexBuffer->myByteOffset = 0;
		myVertexBuffer->myStartSlot = 0;
		myVertexBuffer->myNumberOfBuffers = 1;


		ZeroMemory(myVertexBufferDesc, sizeof(myVertexBufferDesc));
		myVertexBufferDesc->Usage = static_cast<D3D11_USAGE>(aBufferUsage);
		myVertexBufferDesc->BindFlags = D3D11_BIND_VERTEX_BUFFER;
		myVertexBufferDesc->CPUAccessFlags = aCPUUsage;
		myVertexBufferDesc->MiscFlags = 0;
		myVertexBufferDesc->StructureByteStride = 0;
	}

	void BaseModel::InitIndexBuffer()
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

	void BaseModel::InitSurface(const std::string& aResourceName, const std::string& aFileName)
	{
		Surface* surface = new Surface();
		
		surface->SetEffect(myEffect);
		surface->SetIndexCount(0);
		surface->SetIndexStart(0);
		surface->SetVertexCount(0);
		surface->SetVertexStart(0);
		surface->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		surface->SetTexture(aResourceName, aFileName, true);
		mySurfaces.Add(surface);
	}

	void BaseModel::InitBlendState()
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
			DL_ASSERT("BaseModel::InitBlendState: Failed to CreateBlendState");
		}

		Engine::GetInstance()->SetDebugName(myBlendState, "BaseModel::myBlendState");
	}

	void BaseModel::SetupVertexBuffer(int aVertexCount, int aVertexSize, char* aVertexData)
	{
		if (myVertexBuffer->myVertexBuffer != nullptr)
			myVertexBuffer->myVertexBuffer->Release();

		myVertexBufferDesc->ByteWidth = aVertexSize * aVertexCount;
		myInitData->pSysMem = aVertexData;


		HRESULT hr = Engine::GetInstance()->GetDevice()->CreateBuffer(myVertexBufferDesc, myInitData
			, &myVertexBuffer->myVertexBuffer);
		if (FAILED(hr) != S_OK)
		{
			DL_ASSERT("BaseModel::SetupVertexBuffer: Failed to SetupVertexBuffer");
		}

		Engine::GetInstance()->SetDebugName(myVertexBuffer->myVertexBuffer, "BaseModel::myVertexBuffer->myVertexBuffer");
	}

	void BaseModel::SetupIndexBuffer(int aIndexCount, char* aIndexData)
	{
		if (myIndexBuffer->myIndexBuffer != nullptr)
			myIndexBuffer->myIndexBuffer->Release();

		myIndexBufferDesc->ByteWidth = sizeof(UINT) * aIndexCount;
		myInitData->pSysMem = aIndexData;


		HRESULT hr = Engine::GetInstance()->GetDevice()->CreateBuffer(myIndexBufferDesc, myInitData,
			&myIndexBuffer->myIndexBuffer);
		if (FAILED(hr) != S_OK)
		{
			DL_ASSERT("BaseModel::SetupIndexBuffer: Failed to SetupIndexBuffer");
		}

		Engine::GetInstance()->SetDebugName(myIndexBuffer->myIndexBuffer, "BaseModel::myIndexBuffer->myIndexBuffer");
	}

	void BaseModel::OnEffectLoad()
	{
		for (int i = 0; i < mySurfaces.Size(); ++i)
		{
			mySurfaces[i]->ReloadSurface();
		}
	}
}