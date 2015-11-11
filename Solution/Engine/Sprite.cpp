#include "stdafx.h"

#include <D3D11.h>
#include <d3dx11effect.h>
#include "Effect2D.h"
#include "Enums.h"
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

		InitInputLayout(vertexDesc, ARRAYSIZE(vertexDesc));
		InitVertexBuffer(sizeof(VertexPosUV), D3D11_USAGE_IMMUTABLE, 0);
		InitIndexBuffer();
		InitSurface("Texture", aSpritePath);
		InitBlendState();

		ZeroMemory(&myInitData, sizeof(myInitData));

		CreateVertices();
	}

	Sprite::~Sprite()
	{

	}

	void Sprite::Render(const CU::Vector2<float>& aPosition, const CU::Vector2<float>& aScale
		, const CU::Vector4<float>& aColor)
	{
		Engine::GetInstance()->SetDepthBufferState(eDepthStencilType::Z_DISABLED);

		myPosition = aPosition;
		myScale = aScale;

		float blendFactor[4];
		blendFactor[0] = 0.f;
		blendFactor[1] = 0.f;
		blendFactor[2] = 0.f;
		blendFactor[3] = 0.f;

		myEffect->SetBlendState(myBlendState.Get(), blendFactor);
		static_cast<Effect2D*>(myEffect)->SetProjectionMatrix(Engine::GetInstance()->GetOrthogonalMatrix());
		static_cast<Effect2D*>(myEffect)->SetPosAndScale(aPosition, aScale);
		static_cast<Effect2D*>(myEffect)->SetColor(aColor);

		BaseModel::Render();
	}

	void Sprite::CreateVertices()
	{
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

		mySurfaces[0]->SetVertexCount(vertices.Size());
		mySurfaces[0]->SetIndexCount(indices.Size());
	}
}