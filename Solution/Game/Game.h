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

	class ParticleEmitterData;
	class ParticleEmitterInstance;
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
	void operator=(Game& aApp) = delete;
	void UpdateSubSystems();
	void Render();

	CU::InputWrapper* myInputWrapper;

	Easy3D::Camera* myCamera;
	Easy3D::DebugMenu* myDebugMenu;
	Easy3D::ParticleEmitterData* myEmitterData;
	Easy3D::ParticleEmitterInstance* myEmitterInstance;
	Easy3D::Instance* myArm;

	float myDeltaTime;

	int myFPS;
	int myMemoryUsage;
	float myCPUUsage;
	bool myTestBool;
	CU::Vector3<float> myEmitterPosition;


};