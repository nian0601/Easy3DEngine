#pragma once
#include <GrowingArray.h>
#include <unordered_map>
namespace Easy3D
{
	class Camera;
	class Effect3D;
	class Model;

	class RenderProcessTarget
	{
	public:
		RenderProcessTarget();

		void SetCamera(Camera* aCamera);
		void AddModel(Model* aModel, const CU::Matrix44<float>& aWorldMatrix);

		void RenderNormal();
		void RenderInstanced();

	private:
		Camera* myCamera;
		std::unordered_map<Model*, CU::GrowingArray<CU::Matrix44<float>>> myRenderInfo;
	};
}