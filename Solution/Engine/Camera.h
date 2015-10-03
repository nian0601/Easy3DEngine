#pragma once
#include <Matrix44.h>

namespace Easy3D
{
	class Camera
	{
	public:
		Camera();
		~Camera();

		void OnResize(const int aWidth, const int aHeight);

		const CU::Matrix44<float>& GetOrientation() const;
		const CU::Vector3<float>& GetPosition() const;
		const CU::Matrix44<float>& GetProjection() const;

		void SetOrientation(const CU::Matrix44<float>& aOrientation);
		void SetPosition(const CU::Vector3<float>& aPosition);

		void RotateX(const float aDegrees);
		void RotateY(const float aDegrees);
		void RotateZ(const float aDegrees);

		void MoveForward(const float aDistance);
		void MoveRight(const float aDistance);


	private:
		void operator= (const Camera&) = delete;

		CU::Matrix44<float> myOrientation;
		CU::Matrix44<float> myProjectionMatrix;
		CU::Vector3<float> myPosition;
	};

}