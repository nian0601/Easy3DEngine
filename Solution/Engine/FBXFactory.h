#pragma once
#include <vector>
#include <unordered_map>

struct ModelData;

class FBXLoader;
class FbxModelData;

namespace Easy3D
{
	class Model;
	class Effect3D;

	class FBXFactory
	{
	public:
		FBXFactory();
		~FBXFactory();

		Model* LoadModel(const char* aFilePath, Effect3D* aEffect);
	private:
		void FillData(ModelData* someData, Model* outData, Effect3D* aEffect);
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