#include "stdafx.h"

#include "Animation.h"
#include "AnimationNode.h"
#include "Effect3D.h"
#include "FBXFactory.h"
#include "FBX/FbxLoader.h"
#include "HierarchyBone.h"
#include "Matrix44.h"
#include "Model.h"
#include "Surface.h"
#include "IndexBufferWrapper.h"
#include "VertexBufferWrapper.h"
#include "VertexDataWrapper.h"
#include "VertexIndexWrapper.h"

#include "AnimationCurve.h"
#include "TransformationNodeCurves.h"


Easy3D::FBXFactory::FBXFactory()
{
	CU::Matrix44f test;
	test.SetPos(CU::Vector3f(1, 1, 1));
	CU::Matrix44f resultMatrix;

	myLoader = new FBXLoader();
}
	
Easy3D::FBXFactory::~FBXFactory()
{
	delete myLoader;

	for (unsigned int i = 0; i < myFBXData.size(); ++i)
	{
		delete myFBXData[i];
		myFBXData[i] = nullptr;
	}

	for (auto it = myModels.begin(); it != myModels.end(); ++it)
	{
		delete it->second;
		it->second = nullptr;
	}
}

void Easy3D::FBXFactory::FillData(ModelData* someData, Model* outData, Effect3D* aEffect)
{
	VertexIndexWrapper* indexWrapper = new VertexIndexWrapper();
	indexWrapper->myFormat = DXGI_FORMAT_R32_UINT;
	unsigned int* indexData = new unsigned int[someData->myIndexCount];
	memcpy(indexData, someData->myIndicies, someData->myIndexCount*sizeof(unsigned int));
	indexWrapper->myIndexData = (char*)indexData;
	indexWrapper->mySize = someData->myIndexCount*sizeof(unsigned int);
	indexWrapper->myNumberOfIndices = someData->myIndexCount;
	outData->myIndexBaseData = indexWrapper;

	VertexDataWrapper* vertexData = new VertexDataWrapper();
	someData->myVertexBuffer;
	int sizeOfBuffer = someData->myVertexCount*someData->myVertexStride*sizeof(float);
	char* vertexRawData = new char[sizeOfBuffer];
	memcpy(vertexRawData, someData->myVertexBuffer, sizeOfBuffer);
	vertexData->myVertexData = vertexRawData;
	vertexData->myNumberOfVertices = someData->myVertexCount;
	vertexData->mySize = sizeOfBuffer;
	vertexData->myStride = someData->myVertexStride*sizeof(float);

	outData->myVertexBaseData = vertexData;
	outData->myVertexFormat.Init(5);

	for (int i = 0; i < someData->myLayout.Size(); ++i)
	{
		auto currentLayout = someData->myLayout[i];
		D3D11_INPUT_ELEMENT_DESC* desc = new D3D11_INPUT_ELEMENT_DESC();
		desc->SemanticIndex = 0;
		desc->AlignedByteOffset = currentLayout.myOffset;
		desc->InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc->InputSlot = 0;
		desc->InstanceDataStepRate = 0;

		if (currentLayout.myType == ModelData::VERTEX_POS)
		{
			desc->SemanticName = "POSITION";
			desc->Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_NORMAL)
		{
			desc->SemanticName = "NORMAL";
			desc->Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_UV)
		{
			desc->SemanticName = "TEXCOORD";
			desc->Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_BINORMAL)
		{
			desc->SemanticName = "BINORMAL";
			desc->Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_TANGENT)
		{
			desc->SemanticName = "TANGENT";
			desc->Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_SKINWEIGHTS)
		{
			desc->SemanticName = "WEIGHTS";
			desc->Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_BONEID)
		{
			desc->SemanticName = "BONES";
			desc->Format = DXGI_FORMAT_R32G32B32A32_SINT;
		}
		outData->myVertexFormat.Add(desc);
	}
	outData->myIsNULLObject = false;
		
	Surface surface;
	surface.SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	surface.SetIndexCount(someData->myIndexCount);
	surface.SetVertexStart(0);
	surface.SetIndexStart(0);
	surface.SetVertexCount(someData->myVertexCount);
	surface.SetEffect(aEffect);
	bool useSRGB = true;
	for (unsigned int i = 0; i < someData->myTextures.size(); ++i)
	{
		auto& currentTexture = someData->myTextures[i];
			
		std::string resourceName;
		if (currentTexture.myType == DIFFUSE)
		{
			resourceName = "AlbedoTexture";
		}
		if (currentTexture.myType == NORMALMAP)
		{
			resourceName = "NormalTexture";
			useSRGB = false;
		}
		if (currentTexture.myType == ROUGHNESS)
		{
			resourceName = "RoughnessTexture";
		}
		if (currentTexture.myType == SUBSTANCE)
		{
			resourceName = "MetalnessTexture";
		}
		if (currentTexture.myType == AO)
		{
			resourceName = "AOTexture";
		}
		surface.SetTexture(resourceName, currentTexture.myFileName, useSRGB);
	}
	outData->mySurfaces.Add(new Surface(surface));
}

void Easy3D::FBXFactory::FillAnimationData(FbxModelData* someData, Model* outData)
{
	TransformationNodeCurves* nodeCurves = new TransformationNodeCurves();
	Easy3D::KeyFrame frame;
	fbxsdk::FbxAnimCurve* fbxCurve = nullptr;

	float startTime = FLT_MAX;
	float stopTime = FLT_MIN;

	//RotationCurves
	for (int i = 0; i < 3; ++i)
	{
		AnimationCurve* curve = new AnimationCurve();
		fbxCurve = someData->myAnimationCurves->myRotationCurve[i];
		int keyCount = fbxCurve->KeyGetCount();

		for (int j = 0; j < keyCount; ++j)
		{
			frame.myTime = static_cast<float>(fbxCurve->KeyGetTime(j).GetSecondDouble());
			frame.myValue = fbxCurve->KeyGetValue(j);
			curve->AddKeyFrame(frame);
		}

		nodeCurves->SetRotationCurve(i, curve);
		curve->FinalizeCurve();

		fbxsdk::FbxTimeSpan span;
		fbxCurve->GetTimeInterval(span);
		float start = static_cast<float>(span.GetStart().GetSecondDouble());
		float stop = static_cast<float>(span.GetStop().GetSecondDouble());

		startTime = fminf(start, startTime);
		stopTime = fmaxf(stop, stopTime);
	}
	 
	//TransalationCurves
	for (int i = 0; i < 3; ++i)
	{
		AnimationCurve* curve = new AnimationCurve();
		fbxCurve = someData->myAnimationCurves->myTtranslationCurve[i];
		int keyCount = fbxCurve->KeyGetCount();

		for (int j = 0; j < keyCount; ++j)
		{
			frame.myTime = static_cast<float>(fbxCurve->KeyGetTime(j).GetSecondDouble());
			frame.myValue = fbxCurve->KeyGetValue(j);
			curve->AddKeyFrame(frame);
		}

		curve->FinalizeCurve();
		nodeCurves->SetTranslationCurve(i, curve);

		fbxsdk::FbxTimeSpan span;
		fbxCurve->GetTimeInterval(span);
		float start = static_cast<float>(span.GetStart().GetSecondDouble());
		float stop = static_cast<float>(span.GetStop().GetSecondDouble());

		startTime = fminf(start, startTime);
		stopTime = fmaxf(stop, stopTime);
	}
	
	//we are not reading KeyFrameCount at the moment, using -1 until we need the real count!
	nodeCurves->Init(Easy3D::AnimationType::RELATIVE_TRANSFORM, -1, static_cast<int>(someData->myFPS) , startTime, stopTime);
	outData->myTransformation = nodeCurves;
}

void Easy3D::FBXFactory::FillBoneAnimationData(FbxModelData* someData, Model* aOutData)
{
	auto loadedAnimation = someData->myAnimation;
	float animationLenght = 0.f;

	Animation* newAnimation = new Animation();

	HierarchyBone rootBone;
	BuildBoneHierarchy(loadedAnimation->myBones[loadedAnimation->myRootBone], loadedAnimation, rootBone);

	int nrOfbones = static_cast<int>(someData->myAnimation->myBones.size());
	for (int i = 0; i < nrOfbones; ++i)
	{
		Bone& currentBone = someData->myAnimation->myBones[i];
		
		AnimationNode* newNode = new AnimationNode(currentBone.myFrames.size());

		int nrOfFrames = currentBone.myFrames.size();
		for (int j = 0; j < nrOfFrames; ++j)
		{
			AnimationNodeValue newValue;
			auto currentFrame = currentBone.myFrames[j];

			newValue.myTime = currentFrame.myTime;
			newValue.myMatrix = currentFrame.myMatrix;

			newNode->AddValue(newValue);
		}

		newNode->myBoneName = currentBone.myName;
		newAnimation->AddAnimation(newNode);

		newAnimation->SetBoneMatrix(currentBone.myBaseOrientation, i);
		newAnimation->SetBoneBindPose(currentBone.myBindMatrix, i);
		newAnimation->AddBoneName(currentBone.myName);

		newAnimation->SetBindMatrix(loadedAnimation->myBindMatrix);
	}

	newAnimation->SetHierarchy(rootBone);
	animationLenght = someData->myAnimation->myBones[0].myAnimationTime;
	newAnimation->SetAnimationLenght(animationLenght);

	aOutData->myAnimation = newAnimation;
}

void Easy3D::FBXFactory::BuildBoneHierarchy(Bone& aBone, AnimationData* aAnimationData, Easy3D::HierarchyBone& aOutBone)
{
	aOutBone.myBoneID = aBone.myId;
	aOutBone.myBoneName = aBone.myName;
	const int nrOfChildren = static_cast<int>(aBone.myChilds.size());

	if (nrOfChildren > 0)
	{
		aOutBone.myChildren.Init(nrOfChildren);
		for (int i = 0; i < nrOfChildren; ++i)
		{
			HierarchyBone child;
			BuildBoneHierarchy(aAnimationData->myBones[aBone.myChilds[i]], aAnimationData, child);
			aOutBone.myChildren.Add(child);
		}
	}
}

Easy3D::Model* Easy3D::FBXFactory::CreateModel(FbxModelData* someModelData, Effect3D* aEffect)
{
	Model* tempModel = new Model();
	tempModel->SetEffect(aEffect);
		
	if (someModelData->myData)
	{
		FillData(someModelData->myData, tempModel, aEffect);
		tempModel->myOrientation = someModelData->myOrientation;
	}

	if (someModelData->myAnimationCurves != nullptr
		&& someModelData->myAnimationCurves->myRotationCurve[0] != nullptr
		&& someModelData->myAnimationCurves->myTtranslationCurve[0] != nullptr)
	{
		FillAnimationData(someModelData, tempModel);
	}

	if (someModelData->myAnimation != nullptr && someModelData->myAnimation->myRootBone != -1 
		&& someModelData->myAnimation->myBones.size() > 0)
	{
		FillBoneAnimationData(someModelData, tempModel);
	}

	for (int i = 0; i < someModelData->myChilds.Size(); ++i)
	{
		auto currentChild = someModelData->myChilds[i];
		tempModel->AddChild(CreateModel(currentChild, aEffect));
	}
	return tempModel;
}

Easy3D::Model* Easy3D::FBXFactory::LoadModel(const char* aFilePath, Effect3D* aEffect)
{
	if (myModels.find(aFilePath) != myModels.end())
	{
		return myModels[aFilePath];
	}


	FBXData* found = 0;
	for (FBXData* data : myFBXData)
	{ 
		if (data->myPath.compare(aFilePath) == 0)
		{
			found = data;
			break;
		}
	}

	FbxModelData* modelData = nullptr;
	if (found)
	{
		modelData = found->myData;
	}
	else
	{
		FBXData* data = new FBXData();
		FbxModelData* fbxModelData = myLoader->loadModel(aFilePath);
		data->myData = fbxModelData;
		data->myPath = aFilePath;
		myFBXData.push_back(data);
		modelData = data->myData;
	}


	Model* returnModel = CreateModel(modelData, aEffect);

	myModels[aFilePath] = returnModel;

	return returnModel;
}