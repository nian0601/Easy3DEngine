#include "stdafx.h"

#include <D3D11.h>
#include <d3dx11effect.h>
#include "Effect2D.h"
#include "Effect3D.h"
#include "Enums.h"
#include "Font.h"
#include "FontContainer.h"
#include "IndexBufferWrapper.h"
#include "Text.h"
#include "Surface.h"

namespace Easy3D
{
	void Text::Init(const std::string& aFontPath)
	{
		myEffect = Engine::GetInstance()->GetEffectContainer()->Get2DEffect("Data/effect/2D/FontEffect.fx");
		myFont = Engine::GetInstance()->GetFontContainer()->GetFont(aFontPath);
		myCharSize = myFont->GetCharSize();
		myCharSpacing = 17.f;
		myScale = { 1.f, 1.f };

		D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		InitInputLayout(vertexDesc, ARRAYSIZE(vertexDesc));
		InitVertexBuffer(sizeof(VertexPosUV), D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
		InitIndexBuffer();
		InitSurface("Texture", myFont->GetTexture()->GetFileName());
		InitBlendState();

		ZeroMemory(myInitData, sizeof(myInitData));

		myVertices.Init(16);
		myIndices.Init(16);
	}

	void Text::Render(const std::string& aString, const CU::Vector2<float>& aPosition
		, const CU::Vector2<float>& aScale, const CU::Vector4<float>& aColor)
	{
		if (Engine::GetInstance()->myWireframeShouldShow == true)
		{
			Engine::GetInstance()->SetRasterizeState(eRasterizerType::CULL_FRONT);
		}

		if (myLastText != aString)
		{
			ConstructBuffers(aString);
		}

		Engine::GetInstance()->SetDepthBufferState(eDepthStencilType::Z_DISABLED);

		myPosition = aPosition;
		myScale = aScale;

		float blendFactor[4];
		blendFactor[0] = 0.f;
		blendFactor[1] = 0.f;
		blendFactor[2] = 0.f;
		blendFactor[3] = 0.f;

		myEffect->SetBlendState(myBlendState, blendFactor);
		static_cast<Effect2D*>(myEffect)->SetProjectionMatrix(Engine::GetInstance()->GetOrthogonalMatrix());
		static_cast<Effect2D*>(myEffect)->SetPosAndScale(aPosition, aScale);
		static_cast<Effect2D*>(myEffect)->SetColor(aColor);

		BaseModel::Render();
	}


	CU::Vector2<float> Text::GetTextSize(const std::string& aString) const
	{
		int numOfLetters = aString.length();
		CU::Vector2<float> size;


		size.x += (myCharSize.x - myCharSpacing) * numOfLetters;
		size.y = myCharSize.y;

		return size * myScale;
	}

	float Text::GetTextWidth() const
	{
		return myTextWidth * myScale.x;

	}

	void Text::ConstructBuffers(const std::string& aString)
	{
		TIME_FUNCTION;

		myLastText = aString;
		myTextWidth = 0;

		int numOfLetters = aString.length();
		float drawX = 0;
		float drawY = 0;
		float z = 1.f;

		myVertices.RemoveAll();
		myIndices.RemoveAll();
		VertexPosUV vert;
		for (int i = 0; i < numOfLetters; ++i)
		{
			Font::CharacterData charData = myFont->GetCharData(aString[i]);

			float left = drawX;
			float right = left + myCharSize.x;
			float top = drawY;
			float bottom = top - myCharSize.y;


			vert.myPos = CU::Vector3<float>(left, top, z);
			vert.myUV = charData.myTopLeftUV;
			myVertices.Add(vert);

			vert.myPos = CU::Vector3<float>(right, bottom, z);
			vert.myUV = charData.myBottomRightUV;
			myVertices.Add(vert);

			vert.myPos = CU::Vector3<float>(left, bottom, z);
			vert.myUV = { charData.myTopLeftUV.x, charData.myBottomRightUV.y };
			myVertices.Add(vert);

			vert.myPos = CU::Vector3<float>(right, top, z);
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

		SetupVertexBuffer(myVertices.Size(), sizeof(VertexPosUV), reinterpret_cast<char*>(&myVertices[0]));
		SetupIndexBuffer(myIndices.Size(), reinterpret_cast<char*>(&myIndices[0]));

		mySurfaces[0]->SetVertexCount(myVertices.Size());
		mySurfaces[0]->SetIndexCount(myIndices.Size());

		myTextWidth = drawX - myTextWidth;
	}

}