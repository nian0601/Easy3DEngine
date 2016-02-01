#pragma once

#include <bitset>
#include <Matrix.h>
#include <GrowingArray.h>

struct D3D11_VIEWPORT;

namespace Easy3D
{
	class Camera;
	class DebugMenu;
	class ParticleEmitterInstance;
	class Scene;
	class Renderer;
	class Instance;
	class Video;
}

namespace luabridge
{
	struct lua_State;
}

class Game
{
public:
	Game();
	~Game();

	bool Init(HWND& aHwnd, bool aUseInputRecording);
	bool Destroy();

	bool Update();
	

	void Pause();
	void UnPause();
	void OnResize(int aWidth, int aHeight);

private:
	void operator=(Game& aApp) = delete;
	void UpdateSubSystems();
	void Render();
	void ToggleCamera();

	Easy3D::Camera* myCamera;
	Easy3D::DebugMenu* myDebugMenu;
	Easy3D::Scene* myScene;
	Easy3D::Renderer* myRenderer;
	Easy3D::Instance* myMonstersInstance;
	CU::Matrix44<float> myMonstersInstanceOrientation;

	Easy3D::Instance* myMinionsInstance;
	CU::Matrix44<float> myMinionsInstanceOrientation;

	Easy3D::Video* myVideo;

	float myDeltaTime;

	int myFPS;
	int myMemoryUsage;
	float myCPUUsage;
	bool myCameraEnabled;
};