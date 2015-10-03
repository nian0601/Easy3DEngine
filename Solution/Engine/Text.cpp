#include "stdafx.h"

#include <D3D11.h>
#include <d3dx11effect.h>
#include "Effect3D.h"
#include "Font.h"
#include "FontContainer.h"
#include "IndexBufferWrapper.h"
#include "Text.h"
#include "Surface.h"

namespace Easy3D
{
	void Text::Init(const std::string& aFontPath)
	{
		myEffect = Engine::GetInstance()->GetEffectContainer()->Get3DEffect("Data/effect/FontEffect.fx");
		myFont = Engine::GetInstance()->GetFontContainer()->GetFont(aFontPath);
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


		InitVertexBuffer<VertexPosColorUV>();
		InitIndexBuffer();
		InitSurface(myFont->GetTexture()->GetFileName());
		InitBlendState();

		ZeroMemory(myInitData, sizeof(myInitData));

		myVertices.Init(16);
		myIndices.Init(16);
	}

	void Text::Render(const std::string& aString, const float aDrawX, const float aDrawY
		, const CU::Vector4<float>& aColor, const float aScale)
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

		if (Engine::GetInstance()->myWireframeShouldShow == true)
		{
			Engine::GetInstance()->EnableWireframe();
		}
	}


	CU::Vector2<float> Text::GetTextSize(const char* aString) const
	{
		int numOfLetters = static_cast<int>(strlen(aString));
		CU::Vector2<float> size;


		size.x += (myCharSize.x - myCharSpacing) * numOfLetters;
		size.y = myCharSize.y;

		return size;
	}

	float Text::GetTextWidth() const
	{
		return myTextWidth;

	}
	void Text::UpdateSentence(const std::string& aString, const float aDrawX, const float aDrawY,
		const CU::Vector4<float>& aColor, const float aScale)
	{
		TIME_FUNCTION;

		if (strcmp(aString.c_str(), myLastText.c_str()) == 0 && aDrawX == myLastDrawX
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
		int numOfLetters = aString.length();
		float drawX = aDrawX;
		float drawY = aDrawY;
		float z = 1.f;

		myVertices.RemoveAll();
		myIndices.RemoveAll();
		VertexPosColorUV vert;
		vert.myCol = aColor;
		for (int i = 0; i < numOfLetters; ++i)
		{
			Font::CharacterData charData = myFont->GetCharData(aString[i]);

			float left = drawX;
			float right = left + myCharSize.x;
			float top = drawY;
			float bottom = top - myCharSize.y;

			
			vert.myPos = CU::Vector3<float>(left, top, z) * aScale;
			vert.myUV = charData.myTopLeftUV;
			myVertices.Add(vert);

			vert.myPos = CU::Vector3<float>(right, bottom, z) * aScale;
			vert.myUV = charData.myBottomRightUV;
			myVertices.Add(vert);

			vert.myPos = CU::Vector3<float>(left, bottom, z) * aScale;
			vert.myUV = { charData.myTopLeftUV.x, charData.myBottomRightUV.y };
			myVertices.Add(vert);

			vert.myPos = CU::Vector3<float>(right, top, z) * aScale;
			vert.myUV = { charData.myBottomRightUV.x, charData.myTopLeftUV.y };
			myVertices.Add(vert);


			int startIndex = i * 4;
			myIndices.Add(startIndex + 0);
			myIndices.Add(startIndex + 1);
			myIndices.Add(startIndex + 2);
						   
			myIndices.Add(startIndex + 0);
			myIndices.Add(startIndex + 3);
			myIndices.Add(startIndex + 1);


			drawX += myCharSize.x - myCharSpacing;
			z -= 0.001f;
		}

		SetupVertexBuffer(myVertices.Size(), sizeof(VertexPosColorUV), reinterpret_cast<char*>(&myVertices[0]));
		SetupIndexBuffer(myIndices.Size(), reinterpret_cast<char*>(&myIndices[0]));

		mySurface->SetVertexCount(myVertices.Size());
		mySurface->SetIndexCount(myIndices.Size());

		myTextWidth = drawX - myTextWidth;
	}
}