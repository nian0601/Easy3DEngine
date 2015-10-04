#pragma once
#include <Matrix.h>
#include <GrowingArray.h>


namespace CommonUtilities
{
	class InputWrapper;
}

namespace Easy3D
{
	class Camera;
	class DebugMenu;
	class Sprite;
	class Instance;
}

class BulletManager;

class Game
{
public:
	Game();
	~Game();

	bool Init(HWND& aHwnd);
	bool Destroy();

	bool Update();
	

	void Pause();
	void UnPause();
	void OnResize(int aWidth, int aHeight);


private:
	struct InstanceStruct
	{
		Easy3D::Instance* myInstance;
		CU::Vector3<float> myRotationSpeed;
		CU::Vector3<float> myPosition;
	};
	void operator=(Game& aApp) = delete;
	void UpdateSubSystems();
	void Render();
	void CreateCube();

	CU::InputWrapper* myInputWrapper;

	Easy3D::Camera* myCamera;
	Easy3D::Sprite* mySprite;
	CU::GrowingArray<InstanceStruct> myInstances;
	Easy3D::DebugMenu* myDebugMenu;

	CU::Vector4<float> mySpriteColor;
	CU::Vector2<float> mySpritePos;
	CU::Vector2<float> mySpriteScale;

	CU::Vector3<float> myCubeRotation;
	CU::Vector3<float> myCubePosition;

	float myDeltaTime;

	int myFPS;
	int myMemoryUsage;
	float myCPUUsage;
	bool myTestBool;
};