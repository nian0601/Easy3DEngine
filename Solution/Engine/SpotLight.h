#pragma once

class Camera;
class Instance;

namespace Easy3D
{
	class SpotLight
	{
	public:
		void Update();

		const CU::Vector4<float>& GetColor() const;
		void SetColor(const CU::Vector4<float>& aVector);

		const CU::Vector4<float>& GetPosition() const;
		void SetPosition(const CU::Vector4<float>& aPosition);

		const CU::Vector4<float>& GetDir() const;
		void SetDir(const CU::Vector3<float>& aVector);

		void SetRange(float aRange);
		float GetRange() const;

		void SetCone(float aCone);
		float GetCone() const;


		void PerformTransformation(const CU::Matrix44<float>& aTransformation);

	private:
		CU::Vector4<float> myPosition;
		CU::Vector4<float> myOriginalPosition;

		CU::Vector4<float> myDirection;
		CU::Vector3<float> myOriginalDirection;

		CU::Vector4<float> myColor;
		CU::Matrix44<float> myOrientation;

		float myRange;
		float myCone;
	};

	inline void SpotLight::Update()
	{
		myPosition = myOriginalPosition * myOrientation;

		myDirection = myOriginalDirection * myOrientation;
	}

	inline const CU::Vector4<float>& SpotLight::GetColor() const
	{
		return myColor;
	}

	inline void SpotLight::SetColor(const CU::Vector4<float>& aColor)
	{
		myColor = aColor;
	}

	inline const CU::Vector4<float>& SpotLight::GetPosition() const
	{
		return myPosition;
	}

	inline void SpotLight::SetPosition(const CU::Vector4<float>& aPosition)
	{
		myPosition = aPosition;
		myOriginalPosition = aPosition;
	}

	inline const CU::Vector4<float>& SpotLight::GetDir() const
	{
		return myDirection;
	}

	inline void SpotLight::SetDir(const CU::Vector3<float>& aVector)
	{
		myOriginalDirection = aVector;
		myDirection = { myOriginalDirection.x, myOriginalDirection.y, myOriginalDirection.z, 0.f };
	}

	inline void SpotLight::SetRange(float aRange)
	{
		myRange = aRange;
		if (myRange < 0.f)
		{
			myRange = 0.f;
		}
	}

	inline float SpotLight::GetRange() const
	{
		return myRange;
	}

	inline void SpotLight::SetCone(float aCone)
	{
		myCone = aCone;
		if (myCone < 0.01f)
		{
			myCone = 0.01f;
		}
	}

	inline float SpotLight::GetCone() const
	{
		return myCone;
	}

	inline void SpotLight::PerformTransformation(const CU::Matrix44<float>& aTransformation)
	{
		myOrientation = aTransformation * myOrientation;
	}
}