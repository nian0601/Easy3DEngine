#pragma once
#include <vector>
#include <unordered_map>

struct AnimationData;
struct Bone;
struct ModelData;

class FBXLoader;
class FbxModelData;



namespace Easy3D
{
	class Model;
	class Effect3D;
	class HierarchyBone;

	class FBXFactory
	{
	public:
		FBXFactory();
		~FBXFactory();

		Model* LoadModel(const char* aFilePath, Effect3D* aEffect);
	private:
		void FillData(ModelData* someData, Model* outData, Effect3D* aEffect);
		void FillAnimationData(FbxModelData* someData, Model* outData);
		void FillBoneAnimationData(FbxModelData* someData, Model* aOutData);
		void BuildBoneHierarchy(Bone& aBone, AnimationData* aAnimationData, Easy3D::HierarchyBone& aOutBone);
		

		Model* CreateModel(FbxModelData* someModelData, Effect3D* aEffect);

		FBXLoader *myLoader;

		struct FBXData
		{
			FbxModelData* myData;
			std::string myPath;
		};
		std::vector<FBXData*> myFBXData;
		std::unordered_map<std::string, Model*> myModels;
	};
}