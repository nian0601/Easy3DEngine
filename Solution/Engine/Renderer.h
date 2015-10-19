#pragma once

#define MAX_SCENE_COUNT 3

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

	private:
		int mySceneIndex;
		SceneData* myScenes[MAX_SCENE_COUNT];
		Texture* myToBackbufferTexture;
		FullScreenHelper* myFullScreenHelper;
	};
}