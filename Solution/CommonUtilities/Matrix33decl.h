#pragma once

#include "Matrix44decl.h"

namespace CommonUtilities
{
	template<typename T>

	class Matrix33
	{
	public:
		Matrix33();
		Matrix33(const Matrix33<T>& aMatrix);
		Matrix33(const Matrix44<T>& aMatrix);
		~Matrix33();

		T myMatrix[3 * 3];

		static Matrix33<T> CreateRotateAroundX(T aAngleInRadians);
		static Matrix33<T> CreateRotateAroundY(T aAngleInRadians);
		static Matrix33<T> CreateRotateAroundZ(T aAngleInRadians);
		static Matrix33<T> CreateReflectionMatrixAboutAxis(CU::Vector3<T> aReflectionVector);

		void Rotate2D(T aRadian);
		void SetRotation2D(T aRadian);
		void SetPos(const CommonUtilities::Vector2<T>& aPos);
		void SetPos(const CommonUtilities::Vector3<T>& aPos);
		CommonUtilities::Vector2<T> GetPos() const;
		CommonUtilities::Vector3<T> GetPos3() const;

		inline Matrix33<T> operator=(const Matrix33<T> aMatrix)
		{
			for (unsigned char i = 0; i < 9; ++i)
			{
				myMatrix[i] = aMatrix.myMatrix[i];
			}
			return *this;
		}
	};
}