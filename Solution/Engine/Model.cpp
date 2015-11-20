#include "stdafx.h"

#include "Animation.h"
#include <d3dx11effect.h>
#include "Effect3D.h"
#include "Model.h"
#include "Surface.h"
#include "VertexDataWrapper.h"
#include "VertexIndexWrapper.h"
#include "TransformationNode.h"


namespace Easy3D
{

	Model::Model()
		: BaseModel()
		, myChilds(2)
		, myChildTransforms(2)
		, myIsNULLObject(true)
		, myVertexBaseData(nullptr)
		, myIndexBaseData(nullptr)
		, myTransformation(nullptr)
		, myAnimation(nullptr)
	{

	}

	Model::~Model()
	{
		delete myIndexBaseData;
		delete myVertexBaseData;
		delete myAnimation;

		myChilds.DeleteAll();
		myChildTransforms.RemoveAll();
	}

	void Model::Init()
	{
		if (myIsNULLObject == false)
		{
			const int size = myVertexFormat.Size() + 4;
			D3D11_INPUT_ELEMENT_DESC* vertexDesc = new D3D11_INPUT_ELEMENT_DESC[size];
			for (int i = 0; i < myVertexFormat.Size(); ++i)
			{
				vertexDesc[i] = *myVertexFormat[i];
			}

			vertexDesc[myVertexFormat.Size() + 0] = { "myWorld", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
			vertexDesc[myVertexFormat.Size() + 1] = { "myWorld", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
			vertexDesc[myVertexFormat.Size() + 2] = { "myWorld", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 };
			vertexDesc[myVertexFormat.Size() + 3] = { "myWorld", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 };


			InitInputLayout(vertexDesc, size);
			//delete vertexDesc;
			//myVertexFormat.DeleteAll();

			InitVertexBuffer(myVertexBaseData->myStride, D3D11_USAGE_IMMUTABLE, 0);
			InitInstancingBuffer();
			InitIndexBuffer();

			SetupVertexBuffer(myVertexBaseData->myNumberOfVertices
				, myVertexBaseData->myStride, myVertexBaseData->myVertexData);
			SetupInstancingBuffer();
			SetupIndexBuffer(myIndexBaseData->myNumberOfIndices, myIndexBaseData->myIndexData);
		}

		for (int i = 0; i < myChilds.Size(); ++i)
		{
			myChilds[i]->Init();
		}
	}

	void Model::InitCube(const float aWidth, const float aHeight, const float aDepth, CU::Vector4f aColour)
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

	void Model::AddChild(Model* aChild)
	{
		myChilds.Add(aChild);
		myChildTransforms.Add(new TransformationNode(aChild->myOrientation));
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
			float blendFactor[4];
			blendFactor[0] = 0.f;
			blendFactor[1] = 0.f;
			blendFactor[2] = 0.f;
			blendFactor[3] = 0.f;

			myEffect->SetBlendState(NULL, blendFactor);
			static_cast<Effect3D*>(myEffect)->SetWorldMatrix(aOrientation);

			BaseModel::Render();
		}

		/*for (int i = 0; i < myChilds.Size(); ++i)
		{
			myChilds[i]->Render(myChildTransforms[i]->GetTransformationForTime(0) * aOrientation);
		}*/
	}

	void Model::SetGPUState()
	{
		DL_ASSERT_EXP(mySurfaces.Size() < 2, "We do not support several surfaces yet");

		if (myIsNULLObject == true)
		{
			myChilds[0]->SetGPUState();
		}
		else
		{
			BaseModel::SetGPUState();

			mySurfaces[0]->Activate();
		}
	}

	void Model::SetGPUState(const CU::GrowingArray<CU::Matrix44<float>>& someWorldMatrices)
	{
		DL_ASSERT_EXP(mySurfaces.Size() < 2, "We do not support several surfaces yet");

		if (myIsNULLObject == true)
		{
			myChilds[0]->SetGPUState(someWorldMatrices);
		}
		else
		{
			BaseModel::SetGPUState(someWorldMatrices);

			mySurfaces[0]->Activate();
		}
	}

	int Model::GetIndexCount(int aSurfaceIndex)
	{
		if (myIsNULLObject == true)
		{
			return myChilds[0]->GetIndexCount(aSurfaceIndex);
		}
		else
		{
			return mySurfaces[aSurfaceIndex]->GetIndexCount();
		}
		
	}

	int Model::GetVertexStart(int aSurfaceIndex)
	{
		if (myIsNULLObject == true)
		{
			return myChilds[0]->GetVertexStart(aSurfaceIndex);
		}
		else
		{
			return mySurfaces[aSurfaceIndex]->GetVertexStart();
		}
	}
}
