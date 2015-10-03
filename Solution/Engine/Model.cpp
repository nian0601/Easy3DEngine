#include "stdafx.h"

#include <d3dx11effect.h>
#include "Effect3D.h"
#include "Model.h"
#include "Surface.h"
#include "VertexDataWrapper.h"
#include "VertexIndexWrapper.h"


namespace Easy3D
{

	Model::Model()
		: BaseModel()
	{

	}

	Model::~Model()
	{
		delete myIndexBaseData;
		delete myVertexBaseData;
	}

	void Model::Init()
	{
		if (myIsNULLObject == false)
		{
			const int size = myVertexFormat.Size();
			D3D11_INPUT_ELEMENT_DESC* vertexDesc = new D3D11_INPUT_ELEMENT_DESC[size];
			for (int i = 0; i < myVertexFormat.Size(); ++i)
			{
				vertexDesc[i] = *myVertexFormat[i];
			}

			InitInputLayout(vertexDesc, size);
			delete vertexDesc;
			myVertexFormat.DeleteAll();

			InitVertexBuffer(myVertexBaseData->myStride, D3D11_USAGE_IMMUTABLE, 0);
			InitIndexBuffer();

			SetupVertexBuffer(myVertexBaseData->myNumberOfVertices
				, myVertexBaseData->myStride, myVertexBaseData->myVertexData);

			SetupIndexBuffer(myIndexBaseData->myNumberOfIndices, myIndexBaseData->myIndexData);
		}

		for (int i = 0; i < myChilds.Size(); ++i)
		{
			myChilds[i]->Init();
		}
	}

	void Model::InitPolygon()
	{
		myEffect = Engine::GetInstance()->GetEffectContainer()->Get3DEffect("Data/effect/3D/PolygonEffect.fx");

		if (myEffect == nullptr)
		{
			DL_MESSAGE_BOX("Failed to GetEffect", "InitPolygon", MB_ICONWARNING);
		}

		D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		InitInputLayout(vertexDesc, ARRAYSIZE(vertexDesc));


		CU::GrowingArray<VertexPosColor> vertices;
		vertices.Init(3);
		vertices.Add({ { 0.0f, 0.5f, 0.5f }, { 1.f, 0.f, 0.f, 1.f } });
		vertices.Add({ { 0.5f, -0.5f, 0.5 }, { 0.f, 1.f, 0.f, 1.f } });
		vertices.Add({ { -0.5f, -0.5f, 0.5f }, { 0.f, 0.f, 1.f, 1.f } });


		CU::GrowingArray<int> indices(3);
		indices.Add(0);
		indices.Add(1);
		indices.Add(2);

		InitVertexBuffer(sizeof(VertexPosColor), D3D11_USAGE_IMMUTABLE, 0);
		InitIndexBuffer();

		SetupVertexBuffer(vertices.Size(), sizeof(VertexPosColor), reinterpret_cast<char*>(&vertices[0]));
		SetupIndexBuffer(indices.Size(), reinterpret_cast<char*>(&indices[0]));

		Surface surf;
		surf.SetEffect(myEffect);
		surf.SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		surf.SetVertexStart(0);
		surf.SetVertexCount(vertices.Size());
		surf.SetIndexStart(0);
		surf.SetIndexCount(indices.Size());

		mySurfaces.Add(new Surface(surf));

		myIsNULLObject = false;
	}

	void Model::InitCube(const float aWidth, const float aHeight, const float aDepth)
	{
		myEffect = Engine::GetInstance()->GetEffectContainer()->Get3DEffect("Data/effect/3D/CubeEffect.fx");

		if (myEffect == nullptr)
		{
			DL_MESSAGE_BOX("Failed to GetEffect", "InitCube", MB_ICONWARNING);
			DL_ASSERT("InitCube: Failed to GetEffect, myEffect is nullptr.");
		}

		D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		InitInputLayout(vertexDesc, ARRAYSIZE(vertexDesc));


#pragma region Vertices
		CU::GrowingArray<VertexPosNormUV> vertices;
		vertices.Init(24);

		float halfWidth = aWidth / 2.f;
		float halfHeight = aHeight / 2.f;
		float halfDepth = aDepth / 2.f;

		//0 - 3 (Top)
		vertices.Add({ { -halfWidth, halfHeight, -halfDepth }, { 0.f, 1.f, 0.f }, { 0.0f, 0.0f } });
		vertices.Add({ { halfWidth, halfHeight, -halfDepth }, { 0.f, 1.f, 0.f }, { 1.0f, 0.0f } });
		vertices.Add({ { halfWidth, halfHeight, halfDepth }, { 0.f, 1.f, 0.f }, { 1.0f, 1.0f } });
		vertices.Add({ { -halfWidth, halfHeight, halfDepth }, { 0.f, 1.f, 0.f }, { 0.0f, 1.0f } });

		//4 - 7 (Bottom)
		vertices.Add({ { -halfWidth, -halfHeight, -halfDepth }, { 0.f, -1.f, 0.f }, { 0.0f, 0.0f } });
		vertices.Add({ { halfWidth, -halfHeight, -halfDepth }, { 0.f, -1.f, 0.f }, { 1.0f, 0.0f } });
		vertices.Add({ { halfWidth, -halfHeight, halfDepth }, { 0.f, -1.f, 0.f }, { 1.0f, 1.0f } });
		vertices.Add({ { -halfWidth, -halfHeight, halfDepth }, { 0.f, -1.f, 0.f }, { 0.0f, 1.0f } });

		//8 - 11 (Left)
		vertices.Add({ { -halfWidth, -halfHeight, halfDepth }, { -1.f, 0.f, 0.f }, { 0.0f, 0.0f } });
		vertices.Add({ { -halfWidth, -halfHeight, -halfDepth }, { -1.f, 0.f, 0.f }, { 1.0f, 0.0f } });
		vertices.Add({ { -halfWidth, halfHeight, -halfDepth }, { -1.f, 0.f, 0.f }, { 1.0f, 1.0f } });
		vertices.Add({ { -halfWidth, halfHeight, halfDepth }, { -1.f, 0.f, 0.f }, { 0.0f, 1.0f } });

		//12 - 15 (Right)
		vertices.Add({ { halfWidth, -halfHeight, halfDepth }, { 1.f, 0.f, 0.f }, { 0.0f, 0.0f } });
		vertices.Add({ { halfWidth, -halfHeight, -halfDepth }, { 1.f, 0.f, 0.f }, { 1.0f, 0.0f } });
		vertices.Add({ { halfWidth, halfHeight, -halfDepth }, { 1.f, 0.f, 0.f }, { 1.0f, 1.0f } });
		vertices.Add({ { halfWidth, halfHeight, halfDepth }, { 1.f, 0.f, 0.f }, { 0.0f, 1.0f } });

		//16 - 19 (Front)
		vertices.Add({ { -halfWidth, -halfHeight, -halfDepth }, { 0.f, 0.f, -1.f }, { 0.0f, 0.0f } });
		vertices.Add({ { halfWidth, -halfHeight, -halfDepth }, { 0.f, 0.f, -1.f }, { 1.0f, 0.0f } });
		vertices.Add({ { halfWidth, halfHeight, -halfDepth }, { 0.f, 0.f, -1.f }, { 1.0f, 1.0f } });
		vertices.Add({ { -halfWidth, halfHeight, -halfDepth }, { 0.f, 0.f, -1.f }, { 0.0f, 1.0f } });

		//20 - 23 (Back)
		vertices.Add({ { -halfWidth, -halfHeight, halfDepth }, { 0.f, 0.f, 1.f }, { 0.0f, 0.0f } });
		vertices.Add({ { halfWidth, -halfHeight, halfDepth }, { 0.f, 0.f, 1.f }, { 1.0f, 0.0f } });
		vertices.Add({ { halfWidth, halfHeight, halfDepth }, { 0.f, 0.f, 1.f }, { 1.0f, 1.0f } });
		vertices.Add({ { -halfWidth, halfHeight, halfDepth }, { 0.f, 0.f, 1.f }, { 0.0f, 1.0f } });
#pragma endregion

#pragma region Indices

		CU::GrowingArray<int> indices(24);
		//Top
		indices.Add(3);
		indices.Add(1);
		indices.Add(0);

		indices.Add(2);
		indices.Add(1);
		indices.Add(3);

		//Bottom
		indices.Add(6);
		indices.Add(4);
		indices.Add(5);

		indices.Add(7);
		indices.Add(4);
		indices.Add(6);

		//Left
		indices.Add(11);
		indices.Add(9);
		indices.Add(8);

		indices.Add(10);
		indices.Add(9);
		indices.Add(11);

		//Right
		indices.Add(14);
		indices.Add(12);
		indices.Add(13);

		indices.Add(15);
		indices.Add(12);
		indices.Add(14);

		//Front
		indices.Add(19);
		indices.Add(17);
		indices.Add(16);

		indices.Add(18);
		indices.Add(17);
		indices.Add(19);

		//Back
		indices.Add(22);
		indices.Add(20);
		indices.Add(21);

		indices.Add(23);
		indices.Add(20);
		indices.Add(22);

#pragma endregion


		InitVertexBuffer(sizeof(VertexPosNormUV), D3D11_USAGE_IMMUTABLE, 0);
		InitIndexBuffer();

		SetupVertexBuffer(vertices.Size(), sizeof(VertexPosNormUV), reinterpret_cast<char*>(&vertices[0]));
		SetupIndexBuffer(indices.Size(), reinterpret_cast<char*>(&indices[0]));

		Surface surf;
		surf.SetEffect(myEffect);
		surf.SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		surf.SetVertexStart(0);
		surf.SetVertexCount(vertices.Size());
		surf.SetIndexStart(0);
		surf.SetIndexCount(indices.Size());
		surf.SetTexture("DiffuseTexture", "Data/resources/texture/seafloor.dds", true);

		mySurfaces.Add(new Surface(surf));

		myIsNULLObject = false;
	}

	void Model::InitLightCube(const float aWidth, const float aHeight, const float aDepth, CU::Vector4f aColour)
	{
		myEffect = Engine::GetInstance()->GetEffectContainer()->Get3DEffect("Data/effect/3D/CubeColored.fx");

		if (myEffect == nullptr)
		{
			DL_MESSAGE_BOX("Failed to GetEffect", "InitLightCube", MB_ICONWARNING);
		}

		D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		InitInputLayout(vertexDesc, ARRAYSIZE(vertexDesc));


#pragma region Vertices
		CU::GrowingArray<VertexPosColor> vertices;
		vertices.Init(24);

		float halfWidth = aWidth / 2.f;
		float halfHeight = aHeight / 2.f;
		float halfDepth = aDepth / 2.f;

		//0 - 3 (Top)
		vertices.Add({ { -halfWidth, halfHeight, -halfDepth }, aColour });
		vertices.Add({ { halfWidth, halfHeight, -halfDepth }, aColour });
		vertices.Add({ { halfWidth, halfHeight, halfDepth }, aColour });
		vertices.Add({ { -halfWidth, halfHeight, halfDepth }, aColour });

		//4 - 7 (Bottom)
		vertices.Add({ { -halfWidth, -halfHeight, -halfDepth }, aColour });
		vertices.Add({ { halfWidth, -halfHeight, -halfDepth }, aColour });
		vertices.Add({ { halfWidth, -halfHeight, halfDepth }, aColour });
		vertices.Add({ { -halfWidth, -halfHeight, halfDepth }, aColour });

		//8 - 11 (Left)
		vertices.Add({ { -halfWidth, -halfHeight, halfDepth }, aColour });
		vertices.Add({ { -halfWidth, -halfHeight, -halfDepth }, aColour });
		vertices.Add({ { -halfWidth, halfHeight, -halfDepth }, aColour });
		vertices.Add({ { -halfWidth, halfHeight, halfDepth }, aColour });

		//12 - 15 (Right)
		vertices.Add({ { halfWidth, -halfHeight, halfDepth }, aColour });
		vertices.Add({ { halfWidth, -halfHeight, -halfDepth }, aColour });
		vertices.Add({ { halfWidth, halfHeight, -halfDepth }, aColour });
		vertices.Add({ { halfWidth, halfHeight, halfDepth }, aColour });

		//16 - 19 (Front)
		vertices.Add({ { -halfWidth, -halfHeight, -halfDepth }, aColour });
		vertices.Add({ { halfWidth, -halfHeight, -halfDepth }, aColour });
		vertices.Add({ { halfWidth, halfHeight, -halfDepth }, aColour });
		vertices.Add({ { -halfWidth, halfHeight, -halfDepth }, aColour });

		//20 - 23 (Back)
		vertices.Add({ { -halfWidth, -halfHeight, halfDepth }, aColour });
		vertices.Add({ { halfWidth, -halfHeight, halfDepth }, aColour });
		vertices.Add({ { halfWidth, halfHeight, halfDepth }, aColour });
		vertices.Add({ { -halfWidth, halfHeight, halfDepth }, aColour });
#pragma endregion

#pragma region Indices

		CU::GrowingArray<int> indices(24);
		//Top
		indices.Add(3);
		indices.Add(1);
		indices.Add(0);

		indices.Add(2);
		indices.Add(1);
		indices.Add(3);

		//Bottom
		indices.Add(6);
		indices.Add(4);
		indices.Add(5);

		indices.Add(7);
		indices.Add(4);
		indices.Add(6);

		//Left
		indices.Add(11);
		indices.Add(9);
		indices.Add(8);

		indices.Add(10);
		indices.Add(9);
		indices.Add(11);

		//Right
		indices.Add(14);
		indices.Add(12);
		indices.Add(13);

		indices.Add(15);
		indices.Add(12);
		indices.Add(14);

		//Front
		indices.Add(19);
		indices.Add(17);
		indices.Add(16);

		indices.Add(18);
		indices.Add(17);
		indices.Add(19);

		//Back
		indices.Add(22);
		indices.Add(20);
		indices.Add(21);

		indices.Add(23);
		indices.Add(20);
		indices.Add(22);

#pragma endregion


		InitVertexBuffer(sizeof(VertexPosColor), D3D11_USAGE_IMMUTABLE, 0);
		InitIndexBuffer();

		SetupVertexBuffer(vertices.Size(), sizeof(VertexPosColor), reinterpret_cast<char*>(&vertices[0]));
		SetupIndexBuffer(indices.Size(), reinterpret_cast<char*>(&indices[0]));

		Surface surf;
		surf.SetEffect(myEffect);
		surf.SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		surf.SetVertexStart(0);
		surf.SetVertexCount(vertices.Size());
		surf.SetIndexStart(0);
		surf.SetIndexCount(indices.Size());
		surf.SetTexture("DiffuseTexture", "Data/resources/texture/seafloor.dds", true);

		mySurfaces.Add(new Surface(surf));

		myIsNULLObject = false;
	}

	void Model::InitGeometry(const MeshData& aMeshData)
	{
		myEffect = Engine::GetInstance()->GetEffectContainer()->Get3DEffect("Data/effect/3D/GeometryEffect.fx");

		if (myEffect == nullptr)
		{
			DL_MESSAGE_BOX("Failed to GetEffect", "InitGeometry", MB_ICONWARNING);
		}

		D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		InitInputLayout(vertexDesc, ARRAYSIZE(vertexDesc));


		CU::GrowingArray<VertexPosNormColor> vertices;
		vertices.Init(aMeshData.myVertices.Size());
		for (int i = 0; i < aMeshData.myVertices.Size(); ++i)
		{
			VertexPosNormColor vertex;
			vertex.myPos = aMeshData.myVertices[i].myPosition;
			vertex.myNorm = aMeshData.myVertices[i].myNormal;
			vertex.myColor = aMeshData.myVertices[i].myColor;

			vertices.Add(vertex);
		}

		CU::GrowingArray<int> indices(24);
		for (int i = 0; i < aMeshData.myIndices.Size(); ++i)
		{
			indices.Add(aMeshData.myIndices[i]);
		}

		InitVertexBuffer(sizeof(VertexPosNormColor), D3D11_USAGE_IMMUTABLE, 0);
		InitIndexBuffer();

		SetupVertexBuffer(vertices.Size(), sizeof(VertexPosNormColor), reinterpret_cast<char*>(&vertices[0]));
		SetupIndexBuffer(indices.Size(), reinterpret_cast<char*>(&indices[0]));

		Surface surf;
		surf.SetEffect(myEffect);
		surf.SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		surf.SetVertexStart(0);
		surf.SetVertexCount(vertices.Size());
		surf.SetIndexStart(0);
		surf.SetIndexCount(indices.Size());

		mySurfaces.Add(new Surface(surf));

		myIsNULLObject = false;
	}

	void Model::AddChild(Model* aChild)
	{
		myChilds.Add(aChild);
		myChildTransforms.Add(aChild->myOrientation);
	}

	Effect3D* Model::GetEffect()
	{
		return static_cast<Effect3D*>(myEffect);
	}

	void Model::SetEffect(Effect3D* aEffect)
	{
		myEffect = aEffect;
		myEffect->AddListener(this);

		for (int i = 0; i < mySurfaces.Size(); ++i)
		{
			mySurfaces[i]->SetEffect(myEffect);
		}
	}

	void Model::Render(const CU::Matrix44<float>& aOrientation)
	{
		if (myIsNULLObject == false)
		{
			TIME_FUNCTION;;

			float blendFactor[4];
			blendFactor[0] = 0.f;
			blendFactor[1] = 0.f;
			blendFactor[2] = 0.f;
			blendFactor[3] = 0.f;

			myEffect->SetBlendState(NULL, blendFactor);
			static_cast<Effect3D*>(myEffect)->SetWorldMatrix(aOrientation);

			BaseModel::Render();
		}

		for (int i = 0; i < myChilds.Size(); ++i)
		{
			myChilds[i]->Render(myChildTransforms[i] * aOrientation);
		}
	}
}
