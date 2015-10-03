#include "stdafx.h"

#include <D3D11.h>
#include <d3dx11effect.h>
#include "Effect3D.h"
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
		myEffect = Engine::GetInstance()->GetEffectContainer()->Get3DEffect("Data/effect/SpriteEffect.fx");

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
		InitSurface(aSpritePath);
		InitBlendState();

		ZeroMemory(myInitData, sizeof(myInitData));

		CreateVertices();
	}

	void Sprite::Render(float aDrawX, float aDrawY)
	{
		Engine::GetInstance()->DisableZBuffer();

		float blendFactor[4];
		blendFactor[0] = 0.f;
		blendFactor[1] = 0.f;
		blendFactor[2] = 0.f;
		blendFactor[3] = 0.f;

		myEffect->SetBlendState(myBlendState, blendFactor);
		myEffect->SetViewMatrix(CU::Matrix44<float>());
		myEffect->SetProjectionMatrix(Engine::GetInstance()->GetOrthogonalMatrix());
		myEffect->SetWorldMatrix(myOrienation);

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