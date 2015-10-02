#pragma once
#include <Matrix.h>
#include <GrowingArray.h>


namespace CommonUtilities
{
	class InputWrapper;
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

	bool myLockMouse;

	CU::Vector2<int> myWindowSize;

};