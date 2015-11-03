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
	class RenderProcessTarget;
}

namespace luabridge
{
	struct lua_State;
}

class CollisionManager;
class Entity;
class EntityManager;

enum eGameSettings
{
	NO_INSTANCING,
	EASY_INSTANCING,
	HARDWARE_INSTANCING,
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
	Easy3D::Scene* myInstancedScene;
	int mySceneEffect;

	Easy3D::Renderer* myRenderer;
	Easy3D::RenderProcessTarget* myRenderProcessTarget;
	std::bitset<eGameSettings::_COUNT> mySettings;
	eGameSettings myRenderMode;

	float myDeltaTime;

	int myFPS;
	int myMemoryUsage;
	float myCPUUsage;

	EntityManager* myEntityManager;
	CollisionManager* myCollisionManager;
};