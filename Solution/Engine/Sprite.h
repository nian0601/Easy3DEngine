#pragma once
#include "Base2DModel.h"

namespace Easy3D
{
	class Sprite : public Base2DModel
	{
	public:
		Sprite(const std::string& aSpritePath, const CU::Vector2<float>& aSize
			, const CU::Vector2<float>& aHotSpot = CU::Vector2<float>(0.f, 0.f));

		void Render(float aDrawX, float aDrawY);

	private:
		void CreateVertices();

		CU::Vector2<float> mySize;
		CU::Vector2<float> myHotspot;
	};
}