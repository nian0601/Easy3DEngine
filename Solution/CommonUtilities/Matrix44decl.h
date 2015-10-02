#pragma once

namespace CommonUtilities
{
	template<typename T>

	class Matrix44
	{
	public:
		Matrix44();
		Matrix44(const Matrix44<T>& aMatrix);
		~Matrix44();

		void Init(T* aDataPointer);
		T myMatrix[4 * 4];

		static Matrix44<T> CreateRotateAroundX(T aAngleInRadians);
		static Matrix44<T> CreateRotateAroundY(T aAngleInRadians);
		static Matrix44<T> CreateRotateAroundZ(T aAngleInRadians);
		static Matrix44<T> RotateAroundAxis(const CU::Vector3<T>& aAxis, T aAngleInRadians);
		static Matrix44<T> CreateReflectionMatrixAboutAxis(CU::Vector3<T> aReflectionVector);
		static Matrix44<T> CreateProjectionMatrixLH(T aNearZ, T aFarZ, T aAspectRatio, T aFovAngle);
		static Matrix44<T> CreateOrthogonalMatrixLH(T aWidth, T aHeight, T aNearZ, T aFarZ);

		//not yet impl.
		//void Rotate2D(T aRadian);
		//void SetRotation2D(T aRadian);


		void SetPos(const CommonUtilities::Vector3<T>& aPos);
		void SetPos(const CommonUtilities::Vector4<T>& aPos);
		CommonUtilities::Vector3<T> GetPos() const;
		CommonUtilities::Vector4<T> GetPos4() const;
		CommonUtilities::Vector3<T> GetForward() const;
		CommonUtilities::Vector3<T> GetUp() const;
		CommonUtilities::Vector3<T> GetRight() const;

		inline Matrix44<T> operator=(const Matrix44<T> aMatrix)
		{
			for (unsigned char i = 0; i < 16; ++i)
			{
				myMatrix[i] = aMatrix.myMatrix[i];
			}
			return *this;
		}
	};
}