#pragma once
#include <Matrix.h>
#include <GrowingArray.h>

namespace Easy3D
{
	class Camera;
	class DebugMenu;
	class Scene;
	class Renderer;
}

namespace luabridge
{
	struct lua_State;
}

class CollisionManager;
class Entity;


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

	Easy3D::Camera* myCamera;
	Easy3D::DebugMenu* myDebugMenu;
	Easy3D::Scene* myScene;
	Easy3D::Renderer* myRenderer;

	float myDeltaTime;

	int myFPS;
	int myMemoryUsage;
	float myCPUUsage;

	CU::GrowingArray<Entity*> myEntities;
	CollisionManager* myCollisionManager;
};