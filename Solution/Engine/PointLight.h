#pragma once

class Camera;
class Instance;

namespace Easy3D
{
	class PointLight
	{
	public:
		void Init();
		void Render(Camera* aCamera);
		void Update();

		const CU::Vector4<float>& GetColor() const;
		void SetColor(const CU::Vector4<float>& aVector);

		const CU::Vector4<float>& GetPosition() const;
		void SetPosition(const CU::Vector4<float>& aPosition);

		void SetRange(float aRange);
		float GetRange() const;


		void PerformTransformation(const CU::Matrix44<float>& aTransformation);


	private:
		CU::Vector4<float> myColor;
		CU::Vector4<float> myPosition;
		CU::Vector4<float> myOriginalPosition;

		CU::Matrix44<float> myOrientation;

		float myRange;

		Instance* myInstance;
	};
}

inline void Easy3D::PointLight::Update()
{
	myPosition = myOriginalPosition * myOrientation;
}

inline const CU::Vector4<float>& Easy3D::PointLight::GetColor() const
{
	return myColor;
}

inline void Easy3D::PointLight::SetColor(const CU::Vector4<float>& aColor)
{
	myColor = aColor;
}

inline const CU::Vector4<float>& Easy3D::PointLight::GetPosition() const
{
	return myPosition;
}

inline void Easy3D::PointLight::SetPosition(const CU::Vector4<float>& aPosition)
{
	myPosition = aPosition;
	myOriginalPosition = aPosition;
}

inline void Easy3D::PointLight::SetRange(float aRange)
{
	myRange = aRange;
}

inline float Easy3D::PointLight::GetRange() const
{
	return myRange;
}

inline void Easy3D::PointLight::PerformTransformation(const CU::Matrix44<float>& aTransformation)
{
	myOrientation = aTransformation * myOrientation;
}