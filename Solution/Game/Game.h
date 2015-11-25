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
class EntityManager;

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

	float myDeltaTime;

	int myFPS;
	int myMemoryUsage;
	float myCPUUsage;
	bool myCameraEnabled;

	EntityManager* myEntityManager;
	Entity* myPlayer;
	CollisionManager* myCollisionManager;
};