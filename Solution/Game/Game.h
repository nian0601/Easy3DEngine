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
	void operator=(Game& aApp) = delete;
	void Render();

	CU::InputWrapper* myInputWrapper;

	Easy3D::Camera* myCamera;
	Easy3D::Sprite* mySprite;
	Easy3D::Instance* myCube;
};