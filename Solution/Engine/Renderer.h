#pragma once

#define MAX_SCENE_COUNT 3

struct D3D11_VIEWPORT;

namespace Easy3D
{
	class FullScreenHelper;
	class Scene;
	class Texture;

	struct SceneData;

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void ProcessScene(Scene* aScene, int aEffect);
		void FinalRender();

		void StartFontRendering();
		void EndFontRendering();

	private:
		int mySceneIndex;
		SceneData* myScenes[MAX_SCENE_COUNT];
		Texture* myCombinedScenes;
		Texture* myFontTexture;

		Texture* myFinalTexture;
		FullScreenHelper* myFullScreenHelper;
	};
}