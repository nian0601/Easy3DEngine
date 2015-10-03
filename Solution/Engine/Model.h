#pragma once

#include "BaseModel.h"

struct D3D11_INPUT_ELEMENT_DESC;

namespace Easy3D
{
	class Effect3D;
	struct MeshData;
	struct VertexDataWrapper;
	struct VertexIndexWrapper;

	class Model : public BaseModel
	{
		friend class FBXFactory;

	public:
		Model();
		~Model();

		void Init();
		void InitPolygon();
		void InitCube(float aWidth = 1.f, float aHeight = 1.f, float aDepth = 1.f);
		void InitLightCube(float aWidth = 1.f, float aHeight = 1.f, float aDepth = 1.f
			, CU::Vector4f aColour = { 1.f, 1.f, 1.f, 1.f });
		void InitGeometry(const MeshData& aMeshData);

		void AddChild(Model* aChild);

		Effect3D* GetEffect();
		void SetEffect(Effect3D* aEffect);

		void Render(const CU::Matrix44<float>& aOrientation);

	private:
		bool myIsNULLObject;

		CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC*> myVertexFormat;
		VertexIndexWrapper* myIndexBaseData;
		VertexDataWrapper* myVertexBaseData;

		CU::GrowingArray<Model*> myChilds;
		CU::GrowingArray<CU::Matrix44f> myChildTransforms;
		CU::Matrix44f myOrientation;
	};
}

