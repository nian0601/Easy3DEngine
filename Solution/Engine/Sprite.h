#pragma once
#include "BaseModel.h"

namespace Easy3D
{
	class Sprite : public BaseModel
	{
	public:
		Sprite(const std::string& aSpritePath, const CU::Vector2<float>& aSize
			, const CU::Vector2<float>& aHotSpot = CU::Vector2<float>(0.f, 0.f));
		~Sprite();
		void Render(const CU::Vector2<float>& aPosition, const CU::Vector2<float>& aScale = { 1.f, 1.f }
			, const CU::Vector4<float>& aColor = { 1.f, 1.f, 1.f, 1.f });

	private:
		void CreateVertices();

		CU::Vector2<float> mySize;
		CU::Vector2<float> myHotspot;
	};
}