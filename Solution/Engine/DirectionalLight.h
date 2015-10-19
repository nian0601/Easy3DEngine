#pragma once

namespace Easy3D
{
	class DirectionalLight
	{
	public:
		void Update();

		CU::Vector4<float> GetCurrentDir() const;
		const CU::Vector4<float>& GetColor() const;
		const CU::Vector3<float>& GetDir() const;

		void PerformRotation(const CU::Matrix33<float>& aOrientation);

		void SetColor(const CU::Vector4<float>& aVector);
		void SetDir(const CU::Vector3<float>& aVector);

	private:
		CU::Vector4<float> myColor;
		CU::Vector4<float> myDirection;
		CU::Vector3<float> myOriginalDirection;
		CU::Matrix33<float> myOrientation;
	};
}

inline void Easy3D::DirectionalLight::Update()
{
	CU::Vector3f newDir = myOriginalDirection * myOrientation;

	myDirection = newDir;
}

inline CU::Vector4<float> Easy3D::DirectionalLight::GetCurrentDir() const
{
	return myDirection;
}

inline const CU::Vector4<float>& Easy3D::DirectionalLight::GetColor() const
{
	return myColor;
}

inline const CU::Vector3<float>& Easy3D::DirectionalLight::GetDir() const
{
	return myOriginalDirection;
}

 
inline void Easy3D::DirectionalLight::PerformRotation(const CU::Matrix33<float>& aOrientation)
{
	myOrientation = aOrientation * myOrientation;
}

 
inline void Easy3D::DirectionalLight::SetColor(const CU::Vector4<float>& aVector)
{
	myColor = aVector;
}

inline void Easy3D::DirectionalLight::SetDir(const CU::Vector3<float>& aVector)
{
	myOriginalDirection = CU::GetNormalized(aVector);
	myDirection = aVector;
}