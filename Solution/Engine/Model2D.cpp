#include "stdafx.h"
#include "Model2D.h"
#include <D3D11.h>
#include <d3dx11effect.h>
#include "DebugDataDisplay.h"
#include "Camera.h"
#include "Effect.h"
#include "EffectContainer.h"
#include "Engine.h"
#include "VertexBufferWrapper.h"
#include "IndexBufferWrapper.h"
#include "Surface.h"
#include "TextureContainer.h"

Easy3D::Model2D::Model2D()
{
	myLastDrawX = -999.f;
	myLastDrawY = -999.f;
	myVertexBufferDesc = new D3D11_BUFFER_DESC();
	myIndexBufferDesc = new D3D11_BUFFER_DESC();
	myInitData = new D3D11_SUBRESOURCE_DATA();
}

Easy3D::Model2D::~Model2D()
{
	myVertexBuffer->myVertexBuffer->Release();
	myIndexBuffer->myIndexBuffer->Release();

	delete myVertexBufferDesc;
	delete myIndexBufferDesc;
	delete myInitData;
}

void Easy3D::Model2D::Init(const std::string& aFileName, const CU::Vector2<float> aTextureSize)
{
	myTextureSize = aTextureSize;
	myEffect = Engine::GetInstance()->GetEffectContainer()->GetEffect("Data/effect/SpriteEffect.fx");

	if (myEffect == nullptr)
	{
		DL_MESSAGE_BOX("Failed to GetEffect", "Model2D::Init", MB_ICONWARNING);
	}

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3DX11_PASS_DESC passDesc;
	myEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);
	HRESULT hr = Engine::GetInstance()->GetDevice()->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &myVertexLayout);
	if (FAILED(hr) != S_OK)
	{
		DL_MESSAGE_BOX("Failed to CreateInputLayout", "Model2D::Init", MB_ICONWARNING);
	}

	myVertices.Init(6);
	myVerticeIndices.Init(6);

	InitVertexBuffer();
	InitIndexBuffer();
	InitSurface(aFileName);
	InitBlendState();

	ZeroMemory(myInitData, sizeof(myInitData));
}

void Easy3D::Model2D::InitVertexBuffer()
{
	myVertexBuffer = new VertexBufferWrapper();
	myVertexBuffer->myStride = sizeof(VertexPosUV);
	myVertexBuffer->myByteOffset = 0;
	myVertexBuffer->myStartSlot = 0;
	myVertexBuffer->myNumberOfBuffers = 1;


	ZeroMemory(myVertexBufferDesc, sizeof(myVertexBufferDesc));
	myVertexBufferDesc->Usage = D3D11_USAGE_DYNAMIC;
	myVertexBufferDesc->BindFlags = D3D11_BIND_VERTEX_BUFFER;
	myVertexBufferDesc->CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	myVertexBufferDesc->MiscFlags = 0;
	myVertexBufferDesc->StructureByteStride = 0;
}

void Easy3D::Model2D::InitIndexBuffer()
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

void Easy3D::Model2D::InitSurface(const std::string& aFileName)
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

void Easy3D::Model2D::InitBlendState()
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

void Easy3D::Model2D::Render(const Camera& aCamera, const float aDrawX, const float aDrawY)
{
	Update(aDrawX, aDrawY);

	Engine::GetInstance()->DisableZBuffer();

	float blendFactor[4];
	blendFactor[0] = 0.f;
	blendFactor[1] = 0.f;
	blendFactor[2] = 0.f;
	blendFactor[3] = 0.f;

	myEffect->SetBlendState(myBlendState, blendFactor);
	myEffect->SetViewMatrix(myIdentityMatrix);
	myEffect->SetProjectionMatrix(aCamera.GetOrthogonal());
	myEffect->SetWorldMatrix(myIdentityMatrix);

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

void Easy3D::Model2D::SetupVertexBuffer()
{
	TIME_FUNCTION

		if (myVertexBuffer->myVertexBuffer != nullptr)
			myVertexBuffer->myVertexBuffer->Release();

	myVertexBufferDesc->ByteWidth = sizeof(VertexPosUV) * myVertices.Size();
	myInitData->pSysMem = reinterpret_cast<char*>(&myVertices[0]);


	HRESULT hr = Engine::GetInstance()->GetDevice()->CreateBuffer(myVertexBufferDesc, myInitData, &myVertexBuffer->myVertexBuffer);
	if (FAILED(hr) != S_OK)
	{
		DL_MESSAGE_BOX("Failed to SetupVertexBuffer", "Model2D::SetupVertexBuffer", MB_ICONWARNING);
	}
}

void Easy3D::Model2D::SetupIndexBuffer()
{
	TIME_FUNCTION

		if (myIndexBuffer->myIndexBuffer != nullptr)
			myIndexBuffer->myIndexBuffer->Release();

	myIndexBufferDesc->ByteWidth = sizeof(UINT) * myVerticeIndices.Size();
	myInitData->pSysMem = reinterpret_cast<char*>(&myVerticeIndices[0]);


	HRESULT hr = Engine::GetInstance()->GetDevice()->CreateBuffer(myIndexBufferDesc, myInitData,
		&myIndexBuffer->myIndexBuffer);
	if (FAILED(hr) != S_OK)
	{
		DL_MESSAGE_BOX("Failed to SetupIndexBuffer", "Model2D::SetupIndexBuffer", MB_ICONWARNING);
	}
}

void Easy3D::Model2D::OnEffectLoad()
{
	mySurface->ReloadSurface();
}

void Easy3D::Model2D::Update(const float aDrawX, const float aDrawY)
{
	TIME_FUNCTION

	if (aDrawX == myLastDrawX && aDrawY == myLastDrawY)
	{
		return;
	}

	myLastDrawX = aDrawX;
	myLastDrawY = aDrawY;

	myVertices.RemoveAll();
	myVerticeIndices.RemoveAll();
	
	float left, right, top, bottom;
	left = aDrawX - (myTextureSize.x / 2.f);
	right = aDrawX + myTextureSize.x - (myTextureSize.x / 2.f);
	top = aDrawY + (myTextureSize.y / 2.f);
	bottom = aDrawY - myTextureSize.y + (myTextureSize.y / 2.f);

	VertexPosUV vert;
	vert.myPos = { left, top, 0.0f };
	vert.myUV = { 0.0f, 0.0f };
	myVertices.Add(vert);

	vert.myPos = { right, bottom, 0.0f };
	vert.myUV = { 1.0f, 1.0f };
	myVertices.Add(vert);

	vert.myPos = { left, bottom, 0.0f };
	vert.myUV = { 0.0f, 1.0f };
	myVertices.Add(vert);

	vert.myPos = { left, top, 0.0f };
	vert.myUV = { 0.0f, 0.0f };
	myVertices.Add(vert);

	vert.myPos = { right, top, 0.0f };
	vert.myUV = { 1.0f, 0.0f };
	myVertices.Add(vert);

	vert.myPos = { right, bottom, 0.0f };
	vert.myUV = { 1.0f, 1.0f };
	myVertices.Add(vert);

	myVerticeIndices.Add(0);
	myVerticeIndices.Add(1);
	myVerticeIndices.Add(2);

	myVerticeIndices.Add(0);
	myVerticeIndices.Add(4);
	myVerticeIndices.Add(1);

	SetupVertexBuffer();
	SetupIndexBuffer();

	mySurface->SetIndexCount(myVerticeIndices.Size());
	mySurface->SetVertexCount(myVertices.Size());
}
