#include "stdafx.h"

#include <D3D11.h>
#include <d3dx11effect.h>
#include "DebugDataDisplay.h"
#include "Camera.h"
#include "Effect.h"
#include "EffectContainer.h"
#include "Engine.h"
#include "Font.h"
#include "Text.h"
#include <string.h>
#include "VertexBufferWrapper.h"
#include "IndexBufferWrapper.h"
#include "Surface.h"

Easy3D::Text::Text()
	: myHasText(false)
	, myLastDrawX(-999.f)
	, myLastDrawY(-999.f)
	, myLastScale(-999.f)
	, myCharSpacing(17.f)
{
	myVertexBufferDesc = new D3D11_BUFFER_DESC();
	myIndexBufferDesc = new D3D11_BUFFER_DESC();
	myInitData = new D3D11_SUBRESOURCE_DATA();
}


Easy3D::Text::~Text()
{
	if (myVertexBuffer->myVertexBuffer != nullptr)
	{
		myVertexBuffer->myVertexBuffer->Release();
	}
	delete myVertexBuffer;

	if (myIndexBuffer->myIndexBuffer != nullptr)
	{
		myIndexBuffer->myIndexBuffer->Release();
	}
	delete myIndexBuffer;

	delete myVertexBufferDesc;
	delete myIndexBufferDesc;
	delete myInitData;
	delete mySurface;
}

void Easy3D::Text::Init(Font* aFont)
{
	myEffect = Engine::GetInstance()->GetEffectContainer()->GetEffect("Data/effect/FontEffect.fx");
	myFont = aFont;
	myCharSize = myFont->GetCharSize();

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3DX11_PASS_DESC passDesc;
	myEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);
	HRESULT hr = Engine::GetInstance()->GetDevice()->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &myVertexLayout);
	if (FAILED(hr) != S_OK)
	{
		DL_MESSAGE_BOX("Failed to CreateInputLayout", "Text::Init", MB_ICONWARNING);
	}

	myVertices.Init(6);
	myVerticeIndices.Init(6);

	InitVertexBuffer();
	InitIndexBuffer();
	InitSurface();
	InitBlendState();

	ZeroMemory(myInitData, sizeof(myInitData));
}

void Easy3D::Text::InitVertexBuffer()
{
	myVertexBuffer = new VertexBufferWrapper();
	myVertexBuffer->myStride = sizeof(VertexPosColorUV);
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

void Easy3D::Text::InitIndexBuffer()
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

void Easy3D::Text::InitSurface()
{
	mySurface = new Surface();

	mySurface->SetEffect(myEffect);
	mySurface->SetIndexCount(0);
	mySurface->SetIndexStart(0);
	mySurface->SetVertexCount(0);
	mySurface->SetVertexStart(0);
	mySurface->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mySurface->SetTexture("DiffuseTexture", myFont->GetTexture());
}

void Easy3D::Text::InitBlendState()
{
	D3D11_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = false;
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

void Easy3D::Text::Render(const char* aString
	, const float aDrawX, const float aDrawY, const CU::Vector4<float>& aColor, const float aScale)
{

	if (Engine::GetInstance()->myWireframeShouldShow == true)
	{
		Engine::GetInstance()->DisableWireframe();
	}

	UpdateSentence(aString, aDrawX, aDrawY, aColor, aScale);

	if (myHasText == false)
		return;



	Engine::GetInstance()->DisableZBuffer();

	float blendFactor[4];
	blendFactor[0] = 0.f;
	blendFactor[1] = 0.f;
	blendFactor[2] = 0.f;
	blendFactor[3] = 0.f;

	myEffect->SetBlendState(myBlendState, blendFactor);
	myEffect->SetViewMatrix(myIdentityMatrix);
	myEffect->SetProjectionMatrix(Engine::GetInstance()->GetOrthogonalMatrix());
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

	if (Engine::GetInstance()->myWireframeShouldShow == true)
	{
		Engine::GetInstance()->EnableWireframe();
	}
}


CU::Vector2<float> Easy3D::Text::GetTextSize(const char* aString) const
{
	int numOfLetters = static_cast<int>(strlen(aString));
	CU::Vector2<float> size;


	size.x += (myCharSize.x - myCharSpacing) * numOfLetters;
	size.y = myCharSize.y;

	return size;
}

float Easy3D::Text::GetTextWidth() const
{
	return myTextWidth;
}

void Easy3D::Text::SetupVertexBuffer()
{
	TIME_FUNCTION


		if (myVertexBuffer->myVertexBuffer != nullptr)
			myVertexBuffer->myVertexBuffer->Release();

	myVertexBufferDesc->ByteWidth = sizeof(VertexPosColorUV) * myVertices.Size();
	myInitData->pSysMem = reinterpret_cast<char*>(&myVertices[0]);


	HRESULT hr = Engine::GetInstance()->GetDevice()->CreateBuffer(myVertexBufferDesc, myInitData, &myVertexBuffer->myVertexBuffer);
	if (FAILED(hr) != S_OK)
	{
		DL_MESSAGE_BOX("Failed to SetupVertexBuffer", "Text::SetupVertexBuffer", MB_ICONWARNING);
	}
}

void Easy3D::Text::SetupIndexBuffer()
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
		DL_MESSAGE_BOX("Failed to SetupIndexBuffer", "Text::SetupIndexBuffer", MB_ICONWARNING);
	}
}


void Easy3D::Text::CreateFirstTri(const CU::Vector3<float>& aDrawPos, const float aScale, const int aIndex,
	const CU::Vector2<float>& aTopLeftUV, const CU::Vector2<float>& aBotRightUV, const CU::Vector4<float>& aColor)
{
	int index = aIndex;

	VertexPosColorUV vert;
	vert.myCol = aColor;
	vert.myPos.z = aDrawPos.z;

	//TopLeft
	vert.myPos.x = aDrawPos.x * aScale;
	vert.myPos.y = aDrawPos.y * aScale;
	vert.myUV = aTopLeftUV;
	myVertices.Add(vert);
	myVerticeIndices.Add(index);
	++index;

	//BottomRight
	vert.myPos.x = (aDrawPos.x + myCharSize.x) * aScale;
	vert.myPos.y = (aDrawPos.y - myCharSize.y) * aScale;
	vert.myUV = aBotRightUV;
	myVertices.Add(vert);
	myVerticeIndices.Add(index);
	++index;


	//BottomLeft
	vert.myPos.x = aDrawPos.x * aScale;
	vert.myPos.y = (aDrawPos.y - myCharSize.y) * aScale;
	vert.myUV.x = aTopLeftUV.x;
	vert.myUV.y = aBotRightUV.y;

	myVertices.Add(vert);
	myVerticeIndices.Add(index);
	++index;
}

void Easy3D::Text::CreateSecondTri(const CU::Vector3<float>& aDrawPos, const float aScale, const int aIndex,
	const CU::Vector2<float>& aTopLeftUV, const CU::Vector2<float>& aBotRightUV, const CU::Vector4<float>& aColor)
{
	int index = aIndex;

	VertexPosColorUV vert;
	vert.myCol = aColor;
	vert.myPos.z = aDrawPos.z;

	//TopLeft
	vert.myPos.x = aDrawPos.x * aScale;
	vert.myPos.y = aDrawPos.y * aScale;
	vert.myUV = aTopLeftUV;
	myVertices.Add(vert);
	myVerticeIndices.Add(index);
	++index;

	//TopRight
	vert.myPos.x = (aDrawPos.x + myCharSize.x) * aScale;
	vert.myPos.y = aDrawPos.y * aScale;
	vert.myUV.x = aBotRightUV.x;
	vert.myUV.y = aTopLeftUV.y;
	myVertices.Add(vert);
	myVerticeIndices.Add(index);
	++index;

	//BottomRight
	vert.myPos.x = (aDrawPos.x + myCharSize.x) * aScale;
	vert.myPos.y = (aDrawPos.y - myCharSize.y) * aScale;
	vert.myUV = aBotRightUV;
	myVertices.Add(vert);
	myVerticeIndices.Add(index);
	++index;
}


void Easy3D::Text::UpdateSentence(const char* aString, const float aDrawX, const float aDrawY,
	const CU::Vector4<float>& aColor, const float aScale)
{
	TIME_FUNCTION

		if (/*myLastText != nullptr && */strcmp(aString, myLastText.c_str()) == 0 && aDrawX == myLastDrawX
			&& aDrawY == myLastDrawY && aScale == myLastScale)
		{
		return;
		}

	myLastText = aString;
	myLastDrawX = aDrawX;
	myLastDrawY = aDrawY;
	myLastScale = aScale;
	myTextWidth = aDrawX;

	myHasText = true;
	int numOfLetters = static_cast<int>(strlen(aString));
	int index = 0;
	float drawX = aDrawX;
	float drawY = aDrawY;
	float z = 1.f;

	myVertices.RemoveAll();
	myVerticeIndices.RemoveAll();
	for (int i = 0; i < numOfLetters; ++i)
	{
		Font::CharacterData charData = myFont->GetCharData(aString[i]);

		CreateFirstTri({ drawX, drawY, z }, aScale, index, charData.myTopLeftUV, charData.myBottomRightUV, aColor);
		index += 3;

		CreateSecondTri({ drawX, drawY, z }, aScale, index, charData.myTopLeftUV, charData.myBottomRightUV, aColor);
		index += 3;


		drawX += myCharSize.x - myCharSpacing;
		z -= 0.001f;
	}

	SetupVertexBuffer();
	SetupIndexBuffer();

	mySurface->SetIndexCount(myVerticeIndices.Size());
	mySurface->SetVertexCount(myVertices.Size());

	myTextWidth = drawX - myTextWidth;
}
