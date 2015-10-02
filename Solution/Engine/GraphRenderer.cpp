#include "stdafx.h"

#include "Camera.h"
#include <d3dx11effect.h>
#include <D3D11.h>
#include "Effect.h"
#include "EffectContainer.h"
#include "GraphRenderer.h"
#include "IndexBufferWrapper.h"
#include "Surface.h"
#include "VertexBufferWrapper.h"

Prism::GraphRenderer::GraphRenderer()
{
	myVertexBufferDesc = new D3D11_BUFFER_DESC();
	myIndexBufferDesc = new D3D11_BUFFER_DESC();
	myInitData = new D3D11_SUBRESOURCE_DATA();
}

Prism::GraphRenderer::~GraphRenderer()
{
	delete myVertexBufferDesc;
	delete myIndexBufferDesc;
	delete myInitData;
}

void Prism::GraphRenderer::Init()
{
	myEffect = Engine::GetInstance()->GetEffectContainer()->GetEffect("Data/effect/GraphEffect.fx");

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3DX11_PASS_DESC passDesc;
	myEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);
	HRESULT hr = Engine::GetInstance()->GetDevice()->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc)
		, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &myVertexLayout);
	if (FAILED(hr) != S_OK)
	{
		DL_MESSAGE_BOX("Failed to CreateInputLayout", "GraphRenderer::Init", MB_ICONWARNING);
	}

	myVertices.Init(6);
	myVerticeIndices.Init(6);

	InitVertexBuffer();
	InitIndexBuffer();
	InitSurface();

	ZeroMemory(myInitData, sizeof(myInitData));
}

void Prism::GraphRenderer::Render(const CU::GrowingArray<float>& aDataArray
	, const CU::Vector2<float>& aTopLeftDrawPos, const CU::Vector2<float>& aGraphSize
	, const float aMaxValue, bool aNewData)
{
	TIME_FUNCTION


	if (Engine::GetInstance()->myWireframeShouldShow == true)
	{
		Engine::GetInstance()->DisableWireframe();
	}

	if (aNewData == true)
	{
		BuildBuffers(aDataArray, aTopLeftDrawPos, aGraphSize, aMaxValue);
	}

	Engine::GetInstance()->DisableZBuffer();


	myEffect->SetBlendState(NULL, NULL);
	myEffect->SetViewMatrix(myIdentityMatrix);
	myEffect->SetProjectionMatrix(Engine::GetInstance()->GetOrthogonalMatrix());
	myEffect->SetWorldMatrix(myIdentityMatrix);

	Engine::GetInstance()->GetContex()->IASetInputLayout(myVertexLayout);
	Engine::GetInstance()->GetContex()->IASetVertexBuffers(myVertexBuffer->myStartSlot
		, myVertexBuffer->myNumberOfBuffers, &myVertexBuffer->myVertexBuffer, &myVertexBuffer->myStride
		, &myVertexBuffer->myByteOffset);
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

	if (Engine::GetInstance()->myWireframeShouldShow == true)
	{
		Engine::GetInstance()->EnableWireframe();
	}
}

void Prism::GraphRenderer::InitVertexBuffer()
{
	myVertexBuffer = new VertexBufferWrapper();
	myVertexBuffer->myStride = sizeof(VertexPosColor);
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

void Prism::GraphRenderer::InitIndexBuffer()
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

void Prism::GraphRenderer::InitSurface()
{
	mySurface = new Surface();

	mySurface->SetEffect(myEffect);
	mySurface->SetIndexCount(0);
	mySurface->SetIndexStart(0);
	mySurface->SetVertexCount(0);
	mySurface->SetVertexStart(0);
	mySurface->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Prism::GraphRenderer::BuildBuffers(const CU::GrowingArray<float>& aDataArray, const CU::Vector2<float>& aTopLeftDrawPos
	, const CU::Vector2<float>& aGraphSize, const float aMaxValue)
{
	TIME_FUNCTION

	float widthPerElement = (aGraphSize.x - aDataArray.Size()) / aDataArray.Size();

	CU::Vector2<float> botLeft = aTopLeftDrawPos;
	botLeft.y -= aGraphSize.y;

	myVertices.RemoveAll();
	myVerticeIndices.RemoveAll();

	CU::Vector2<float> columnSize(widthPerElement, aGraphSize.y);
	int index = 0;
	CreateFirstTri(aTopLeftDrawPos, { aGraphSize.x, 3 }, index, 1.f);
	index += 3;

	CreateSecondTri(aTopLeftDrawPos, { aGraphSize.x, 3 }, index, 1.f);
	index += 3;

	for (int i = 0; i < aDataArray.Size(); ++i)
	{
		float heightCoef = fminf(1.f, (aDataArray[i] / aMaxValue));

		CreateFirstTri(botLeft, columnSize, index, heightCoef);
		index += 3;

		CreateSecondTri(botLeft, columnSize, index, heightCoef);
		index += 3;

		botLeft.x += widthPerElement + 1;
	}

	SetupVertexBuffer();
	SetupIndexBuffer();

	mySurface->SetIndexCount(myVerticeIndices.Size());
	mySurface->SetVertexCount(myVertices.Size());
}

void Prism::GraphRenderer::CreateFirstTri(const CU::Vector2<float>& aBotLeft, const CU::Vector2<float>& aColumSize
	, const int aIndex, const float aHeightCoef)
{
	TIME_FUNCTION

	int index = aIndex;

	VertexPosColor vert;
	vert.myPos.z = 1.f;
	vert.myCol = GetColor(aHeightCoef);

	//TopLeft
	vert.myPos.x = aBotLeft.x;
	vert.myPos.y = aBotLeft.y + (aColumSize.y * aHeightCoef);
	myVertices.Add(vert);
	myVerticeIndices.Add(index);
	++index;

	//BottomRight
	vert.myPos.x = aBotLeft.x + aColumSize.x;
	vert.myPos.y = aBotLeft.y;
	myVertices.Add(vert);
	myVerticeIndices.Add(index);
	++index;


	//BottomLeft
	vert.myPos.x = aBotLeft.x;
	vert.myPos.y = aBotLeft.y;
	myVertices.Add(vert);
	myVerticeIndices.Add(index);
	++index;
}

void Prism::GraphRenderer::CreateSecondTri(const CU::Vector2<float>& aBotLeft, const CU::Vector2<float>& aColumSize
	, const int aIndex, const float aHeightCoef)
{
	TIME_FUNCTION

	int index = aIndex;

	VertexPosColor vert;
	vert.myPos.z = 1.f;
	vert.myCol = GetColor(aHeightCoef);

	//TopLeft
	vert.myPos.x = aBotLeft.x;
	vert.myPos.y = aBotLeft.y + (aColumSize.y * aHeightCoef);
	myVertices.Add(vert);
	myVerticeIndices.Add(index);
	++index;

	//TopRight
	vert.myPos.x = aBotLeft.x + aColumSize.x;
	vert.myPos.y = aBotLeft.y + (aColumSize.y * aHeightCoef);

	myVertices.Add(vert);
	myVerticeIndices.Add(index);
	++index;

	//BottomRight
	vert.myPos.x = aBotLeft.x + aColumSize.x;
	vert.myPos.y = aBotLeft.y;
	myVertices.Add(vert);
	myVerticeIndices.Add(index);
	++index;
}

void Prism::GraphRenderer::SetupVertexBuffer()
{
	TIME_FUNCTION

	if (myVertexBuffer->myVertexBuffer != nullptr)
		myVertexBuffer->myVertexBuffer->Release();

	myVertexBufferDesc->ByteWidth = sizeof(VertexPosColor) * myVertices.Size();
	myInitData->pSysMem = reinterpret_cast<char*>(&myVertices[0]);


	HRESULT hr = Engine::GetInstance()->GetDevice()->CreateBuffer(myVertexBufferDesc, myInitData
		, &myVertexBuffer->myVertexBuffer);
	if (FAILED(hr) != S_OK)
	{
		DL_MESSAGE_BOX("Failed to SetupVertexBuffer", "GraphRenderer::SetupVertexBuffer", MB_ICONWARNING);
	}
}

void Prism::GraphRenderer::SetupIndexBuffer()
{
	TIME_FUNCTION

	if (myIndexBuffer->myIndexBuffer != nullptr)
		myIndexBuffer->myIndexBuffer->Release();

	myIndexBufferDesc->ByteWidth = sizeof(UINT) * myVerticeIndices.Size();
	myInitData->pSysMem = reinterpret_cast<char*>(&myVerticeIndices[0]);


	HRESULT hr = Engine::GetInstance()->GetDevice()->CreateBuffer(myIndexBufferDesc, myInitData
		, &myIndexBuffer->myIndexBuffer);
	if (FAILED(hr) != S_OK)
	{
		DL_MESSAGE_BOX("Failed to SetupIndexBuffer", "GraphRenderer::SetupIndexBuffer", MB_ICONWARNING);
	}
}

CU::Vector4<float> Prism::GraphRenderer::GetColor(const float aHeightCoef)
{
	if (aHeightCoef < 0.25f)
		return{ 0.f, 1.f, 0.f, 1.f };

	if (aHeightCoef < 0.5f)
		return{ 1.f, 1.f, 0.f, 1.f };

	if (aHeightCoef < 1.f)
		return{ 1.f, 0.39f, 0.28f, 1.f };

	return{ 1.f, 0.f, 0.f, 1.f };
}