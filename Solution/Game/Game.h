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
}

namespace luabridge
{
	struct lua_State;
}

class CollisionManager;
class Entity;

enum eGameSettings
{
	SCENE_ONE_RENDER,
	SCENE_ONE_HDR,
	SCENE_ONE_BLOOM,
	SCENE_ONE_MOTION_BLUR,
	SCENE_TWO_RENDER,
	SCENE_TWO_HDR,
	SCENE_TWO_BLOOM,
	SCENE_TWO_MOTION_BLUR,
	_COUNT
};

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

	void ToggleSetting(eGameSettings aSetting);

	Easy3D::Camera* myCamera;
	Easy3D::DebugMenu* myDebugMenu;
	Easy3D::Scene* myScene;
	int mySceneEffect;
	Easy3D::Scene* mySecondScene;
	int mySecondSceneEffect;

	Easy3D::Renderer* myRenderer;
	std::bitset<eGameSettings::_COUNT> mySettings;

	float myDeltaTime;

	int myFPS;
	int myMemoryUsage;
	float myCPUUsage;

	CU::GrowingArray<Entity*> myEntities;
	CollisionManager* myCollisionManager;

	Easy3D::ParticleEmitterInstance* myEmitter;
};