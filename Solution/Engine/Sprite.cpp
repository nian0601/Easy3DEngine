#include "stdafx.h"

#include <D3D11.h>
#include <d3dx11effect.h>
#include "Effect2D.h"
#include "IndexBufferWrapper.h"
#include "Sprite.h"
#include "Surface.h"
#include "VertexBufferWrapper.h"

namespace Easy3D
{
	Sprite::Sprite(const std::string& aSpritePath, const CU::Vector2<float>& aSize
			, const CU::Vector2<float>& aHotSpot)
		: mySize(aSize)
		, myHotspot(aHotSpot)
	{
		myEffect = Engine::GetInstance()->GetEffectContainer()->Get2DEffect("Data/effect/2D/SpriteEffect.fx");
		myEffect->AddListener(this);

		D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		D3DX11_PASS_DESC passDesc;
		myEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);
		HRESULT hr = Engine::GetInstance()->GetDevice()->CreateInputLayout(vertexDesc
			, ARRAYSIZE(vertexDesc), passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &myVertexLayout);
		if (FAILED(hr) != S_OK)
		{
			DL_MESSAGE_BOX("Failed to CreateInputLayout", "Model2D::Init", MB_ICONWARNING);
		}


		InitVertexBuffer<VertexPosUV>();
		InitIndexBuffer();
		InitSurface("Texture", aSpritePath);
		InitBlendState();

		ZeroMemory(myInitData, sizeof(myInitData));

		CreateVertices();
	}

	void Sprite::Render(const CU::Vector2<float>& aPosition, const CU::Vector2<float>& aScale
		, const CU::Vector4<float>& aColor)
	{
		Base2DModel::Render(aPosition, aScale, aColor);
	}

	void Sprite::CreateVertices()
	{
		TIME_FUNCTION;


		CU::GrowingArray<VertexPosUV> vertices(4);
		CU::GrowingArray<int> indices(6);

		float left = 0 - myHotspot.x;
		float right = left + mySize.x;
		float top = 0 + myHotspot.y;
		float bottom = top - mySize.y;

		VertexPosUV vert;
		vert.myPos = { left, top, 0.0f };
		vert.myUV = { 0.0f, 0.0f };
		vertices.Add(vert);

		vert.myPos = { right, bottom, 0.0f };
		vert.myUV = { 1.0f, 1.0f };
		vertices.Add(vert);

		vert.myPos = { left, bottom, 0.0f };
		vert.myUV = { 0.0f, 1.0f };
		vertices.Add(vert);

		vert.myPos = { right, top, 0.0f };
		vert.myUV = { 1.0f, 0.0f };
		vertices.Add(vert);


		indices.Add(0);
		indices.Add(1);
		indices.Add(2);

		indices.Add(0);
		indices.Add(3);
		indices.Add(1);

		SetupVertexBuffer(vertices.Size(), sizeof(VertexPosUV), reinterpret_cast<char*>(&vertices[0]));
		SetupIndexBuffer(indices.Size(), reinterpret_cast<char*>(&indices[0]));

		mySurface->SetVertexCount(vertices.Size());
		mySurface->SetIndexCount(indices.Size());
	}
}