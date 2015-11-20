#pragma once

#include "BaseModel.h"

struct D3D11_INPUT_ELEMENT_DESC;

namespace Easy3D
{
	class Animation;
	class Effect3D;
	class TransformationNode;

	struct MeshData;
	struct VertexDataWrapper;
	struct VertexIndexWrapper;

	class Model : public BaseModel
	{
		friend class FBXFactory;
		friend class Instance;
	public:
		Model();
		~Model();

		void Init();
		void InitCube(float aWidth = 1.f, float aHeight = 1.f, float aDepth = 1.f
			, CU::Vector4f aColour = { 1.f, 1.f, 1.f, 1.f });

		void AddChild(Model* aChild);

		Effect3D* GetEffect();
		void SetEffect(Effect3D* aEffect);

		void Render(const CU::Matrix44<float>& aOrientation);
		void SetGPUState();
		void SetGPUState(const CU::GrowingArray<CU::Matrix44<float>>& someWorldMatrices);

		int GetIndexCount(int aSurfaceIndex);
		int GetVertexStart(int aSurfaceIndex);

	private:
		bool myIsNULLObject;

		CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC*> myVertexFormat;
		VertexIndexWrapper* myIndexBaseData;
		VertexDataWrapper* myVertexBaseData;

		CU::GrowingArray<Model*> myChilds;
		CU::GrowingArray<TransformationNode*> myChildTransforms;
		TransformationNode* myTransformation;
		Animation* myAnimation;
		CU::Matrix44f myOrientation;
	};
}

