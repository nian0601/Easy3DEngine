#pragma once

namespace Easy3D
{
	class Effect;
	class Model;

	class ModelProxy
	{
	public:
		ModelProxy();

		void Render(const CU::Matrix44<float>& aOrientation);

		void SetModel(Model* aModel);

		Effect* GetEffect();
		void SetEffect(Effect* aEffect);

		bool IsLoaded() const;
	private:
		Model* myModel;
	};

	inline bool ModelProxy::IsLoaded() const
	{
		if (myModel == nullptr)
		{
			return false;
		}

		return true;
	}
}

