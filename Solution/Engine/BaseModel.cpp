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
		myEffect = nullptr;

		mySurfaces.Init(1);

		myVertexBuffer = nullptr;
		myIndexBuffer = nullptr;
	}

	BaseModel::~BaseModel()
	{
		delete myVertexBuffer;
		delete myIndexBuffer;
		
		mySurfaces.DeleteAll();
	}

	void BaseModel::Render()
	{
		ID3D11Buffer* buf = myVertexBuffer->myVertexBuffer.Get();
		Engine::GetInstance()->GetContex()->IASetInputLayout(myVertexLayout.Get());
		Engine::GetInstance()->GetContex()->IASetVertexBuffers(myVertexBuffer->myStartSlot
			, myVertexBuffer->myNumberOfBuffers, &buf
			, &myVertexBuffer->myStride, &myVertexBuffer->myByteOffset);
		Engine::GetInstance()->GetContex()->IASetIndexBuffer(myIndexBuffer->myIndexBuffer.Get()
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
		ID3D11InputLayout* input = nullptr;
		HRESULT hr = Engine::GetInstance()->GetDevice()->CreateInputLayout(aVertexDescArray
			, aArraySize, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &input);
		if (FAILED(hr) != S_OK)
		{
			DL_MESSAGE_BOX("Failed to CreateInputLayout", "Model2D::Init", MB_ICONWARNING);
		}

		myVertexLayout.Set(input);

		Engine::GetInstance()->SetDebugName(myVertexLayout.Get(), "BaseModel::myVertexLayout");
	}

	void Easy3D::BaseModel::InitVertexBuffer(int aVertexSize, int aBufferUsage, int aCPUUsage)
	{
		myVertexBuffer = new VertexBufferWrapper();
		myVertexBuffer->myStride = aVertexSize;
		myVertexBuffer->myByteOffset = 0;
		myVertexBuffer->myStartSlot = 0;
		myVertexBuffer->myNumberOfBuffers = 1;


		ZeroMemory(&myVertexBufferDesc, sizeof(myVertexBufferDesc));
		myVertexBufferDesc.Usage = static_cast<D3D11_USAGE>(aBufferUsage);
		myVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		myVertexBufferDesc.CPUAccessFlags = aCPUUsage;
		myVertexBufferDesc.MiscFlags = 0;
		myVertexBufferDesc.StructureByteStride = 0;
	}

	void BaseModel::InitIndexBuffer()
	{
		myIndexBuffer = new IndexBufferWrapper();
		myIndexBuffer->myIndexBufferFormat = DXGI_FORMAT_R32_UINT;
		myIndexBuffer->myByteOffset = 0;


		ZeroMemory(&myIndexBufferDesc, sizeof(myIndexBufferDesc));
		myIndexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		myIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		myIndexBufferDesc.CPUAccessFlags = 0;
		myIndexBufferDesc.MiscFlags = 0;
		myIndexBufferDesc.StructureByteStride = 0;
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

		ID3D11BlendState* state = nullptr;
		HRESULT hr = Engine::GetInstance()->GetDevice()->CreateBlendState(&blendDesc, &state);
		if (FAILED(hr) != S_OK)
		{
			DL_ASSERT("BaseModel::InitBlendState: Failed to CreateBlendState");
		}

		myBlendState.Set(state);
		Engine::GetInstance()->SetDebugName(myBlendState.Get(), "BaseModel::myBlendState");
	}

	void BaseModel::SetupVertexBuffer(int aVertexCount, int aVertexSize, char* aVertexData)
	{
		if (myVertexBuffer->myVertexBuffer.Get() != nullptr)
			myVertexBuffer->myVertexBuffer->Release();

		myVertexBufferDesc.ByteWidth = aVertexSize * aVertexCount;
		myInitData.pSysMem = aVertexData;

		ID3D11Buffer* buf = nullptr;
		HRESULT hr = Engine::GetInstance()->GetDevice()->CreateBuffer(&myVertexBufferDesc, &myInitData
			, &buf);
		if (FAILED(hr) != S_OK)
		{
			DL_ASSERT("BaseModel::SetupVertexBuffer: Failed to SetupVertexBuffer");
		}

		myVertexBuffer->myVertexBuffer.Set(buf);
		Engine::GetInstance()->SetDebugName(myVertexBuffer->myVertexBuffer.Get(), "BaseModel::myVertexBuffer->myVertexBuffer");
	}

	void BaseModel::SetupIndexBuffer(int aIndexCount, char* aIndexData)
	{
		/*if (myIndexBuffer->myIndexBuffer != nullptr)
			myIndexBuffer->myIndexBuffer->Release();*/

		if (myIndexBuffer != nullptr && myIndexBuffer->myIndexBuffer.Get() != nullptr)
			myIndexBuffer->myIndexBuffer->Release();

		myIndexBufferDesc.ByteWidth = sizeof(UINT) * aIndexCount;
		myInitData.pSysMem = aIndexData;

		ID3D11Buffer* buf = myIndexBuffer->myIndexBuffer.Get();
		HRESULT hr = Engine::GetInstance()->GetDevice()->CreateBuffer(&myIndexBufferDesc, &myInitData,
			&buf);
		if (FAILED(hr) != S_OK)
		{
			DL_ASSERT("BaseModel::SetupIndexBuffer: Failed to SetupIndexBuffer");
		}

		myIndexBuffer->myIndexBuffer.Set(buf);
		Engine::GetInstance()->SetDebugName(myIndexBuffer->myIndexBuffer.Get(), "BaseModel::myIndexBuffer->myIndexBuffer");
	}

	void BaseModel::OnEffectLoad()
	{
		for (int i = 0; i < mySurfaces.Size(); ++i)
		{
			mySurfaces[i]->ReloadSurface();
		}
	}
}