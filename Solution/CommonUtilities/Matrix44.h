#pragma once
#include "Vector.h"
#include "Matrix44decl.h"
#include "Matrix33decl.h"
#include "Matrix33.h"
#include <cmath>


namespace CommonUtilities
{
	typedef Matrix44<float> Matrix44f;
	
	template <typename T>
	Matrix44<T>::Matrix44()
	{
		for (unsigned char i = 0; i < 16; ++i)
		{
			myMatrix[i] = T(0);
		}

		myMatrix[0] = T(1);
		myMatrix[5] = T(1);
		myMatrix[10] = T(1);
		myMatrix[15] = T(1);
	}

	template <typename T>
	Matrix44<T>::Matrix44(const Matrix44<T>& aMatrix)
	{
		for (unsigned char i = 0; i < 16; ++i)
		{
			myMatrix[i] = aMatrix.myMatrix[i];
		}
	}

	template <typename T>
	Matrix44<T>::~Matrix44()
	{
	}

	template <typename T>
	void Matrix44<T>::Init(T* aDataPointer)
	{
		for (unsigned char i = 0; i < 16; ++i)
		{
			myMatrix[i] = aDataPointer[i];
		}
	}

	template <typename T>
	Vector3<T> operator*(const Vector3<T>& aVector, const Matrix44<T>& aMatrix)
	{
		return Vector3<T>(
			aVector.x * aMatrix.myMatrix[0] + aVector.y * aMatrix.myMatrix[4] + aVector.z * aMatrix.myMatrix[8],
			aVector.x * aMatrix.myMatrix[1] + aVector.y * aMatrix.myMatrix[5] + aVector.z * aMatrix.myMatrix[9],
			aVector.x * aMatrix.myMatrix[2] + aVector.y * aMatrix.myMatrix[6] + aVector.z * aMatrix.myMatrix[10]);
	}

	template <typename T>
	Vector4<T> operator*(const Vector4<T>& aVector, const Matrix44<T>& aMatrix)
	{
		return Vector4<T>(
			aVector.x * aMatrix.myMatrix[0] + aVector.y * aMatrix.myMatrix[4] + aVector.z * aMatrix.myMatrix[8]  + aVector.w * aMatrix.myMatrix[12],
			aVector.x * aMatrix.myMatrix[1] + aVector.y * aMatrix.myMatrix[5] + aVector.z * aMatrix.myMatrix[9]  + aVector.w * aMatrix.myMatrix[13],
			aVector.x * aMatrix.myMatrix[2] + aVector.y * aMatrix.myMatrix[6] + aVector.z * aMatrix.myMatrix[10] + aVector.w * aMatrix.myMatrix[14],
			aVector.x * aMatrix.myMatrix[3] + aVector.y * aMatrix.myMatrix[7] + aVector.z * aMatrix.myMatrix[11] + aVector.w * aMatrix.myMatrix[15]);
	}

	template <typename T>
	bool operator==(const Matrix44<T>& aFirstMatrix, const Matrix44<T>& aSecondMatrix)
	{
		for (unsigned char i = 0; i < 16; ++i)
		{
			if (aFirstMatrix.myMatrix[i] != aSecondMatrix.myMatrix[i])
			{
				return false;
			}
		}
		return true;
	}

	template <typename T>
	bool operator!=(const Matrix44<T>& aFirstMatrix, const Matrix44<T>& aSecondMatrix)
	{
		return !(operator==(aFirstMatrix, aSecondMatrix));
	}

	template <typename T>
	Matrix44<T> operator+=(Matrix44<T>& aFirstMatrix, const Matrix44<T>& aSecondMatrix)
	{
		for (unsigned char i = 0; i < 16; ++i)
		{
			aFirstMatrix.myMatrix[i] += aSecondMatrix.myMatrix[i];
		}
		return aFirstMatrix;
	}

	template <typename T>
	Matrix44<T> operator+(const Matrix44<T>& aFirstMatrix, const Matrix44<T>& aSecondMatrix)
	{
		Matrix44<T> result(aFirstMatrix);
		result += aSecondMatrix;
		return result;
	}

	template <typename T>
	Matrix44<T> operator-=(Matrix44<T>& aFirstMatrix, const Matrix44<T>& aSecondMatrix)
	{
		for (unsigned char i = 0; i < 16; ++i)
		{
			aFirstMatrix.myMatrix[i] -= aSecondMatrix.myMatrix[i];
		}
		return aFirstMatrix;
	}

	template <typename T>
	Matrix44<T> operator-(const Matrix44<T>& aFirstMatrix, const Matrix44<T>& aSecondMatrix)
	{
		Matrix44<T> result(aFirstMatrix);
		result -= aSecondMatrix;
		return result;
	}

	template <typename T>
	Matrix44<T> operator*(const Matrix44<T>& aFirstMatrix, const Matrix44<T>& aSecondMatrix)
	{
		Matrix44<T> result(aFirstMatrix);
		result *= aSecondMatrix;
		return result;
	}

	template <typename T>
	Matrix44<T> operator*=(Matrix44<T>& aFirstMatrix, const Matrix44<T>& aSecondMatrix)
	{
		Matrix44<T> originalFirst(aFirstMatrix);

		aFirstMatrix.myMatrix[0] =
			originalFirst.myMatrix[0] * aSecondMatrix.myMatrix[0] +
			originalFirst.myMatrix[1] * aSecondMatrix.myMatrix[4] +
			originalFirst.myMatrix[2] * aSecondMatrix.myMatrix[8] +
			originalFirst.myMatrix[3] * aSecondMatrix.myMatrix[12];
		aFirstMatrix.myMatrix[1] =
			originalFirst.myMatrix[0] * aSecondMatrix.myMatrix[1] +
			originalFirst.myMatrix[1] * aSecondMatrix.myMatrix[5] +
			originalFirst.myMatrix[2] * aSecondMatrix.myMatrix[9] +
			originalFirst.myMatrix[3] * aSecondMatrix.myMatrix[13];
		aFirstMatrix.myMatrix[2] =
			originalFirst.myMatrix[0] * aSecondMatrix.myMatrix[2] +
			originalFirst.myMatrix[1] * aSecondMatrix.myMatrix[6] +
			originalFirst.myMatrix[2] * aSecondMatrix.myMatrix[10] +
			originalFirst.myMatrix[3] * aSecondMatrix.myMatrix[14];
		aFirstMatrix.myMatrix[3] =
			originalFirst.myMatrix[0] * aSecondMatrix.myMatrix[3] +
			originalFirst.myMatrix[1] * aSecondMatrix.myMatrix[7] +
			originalFirst.myMatrix[2] * aSecondMatrix.myMatrix[11] +
			originalFirst.myMatrix[3] * aSecondMatrix.myMatrix[15];

		aFirstMatrix.myMatrix[4] =
			originalFirst.myMatrix[4] * aSecondMatrix.myMatrix[0] +
			originalFirst.myMatrix[5] * aSecondMatrix.myMatrix[4] +
			originalFirst.myMatrix[6] * aSecondMatrix.myMatrix[8] +
			originalFirst.myMatrix[7] * aSecondMatrix.myMatrix[12];
		aFirstMatrix.myMatrix[5] =
			originalFirst.myMatrix[4] * aSecondMatrix.myMatrix[1] +
			originalFirst.myMatrix[5] * aSecondMatrix.myMatrix[5] +
			originalFirst.myMatrix[6] * aSecondMatrix.myMatrix[9] +
			originalFirst.myMatrix[7] * aSecondMatrix.myMatrix[13];
		aFirstMatrix.myMatrix[6] =
			originalFirst.myMatrix[4] * aSecondMatrix.myMatrix[2] +
			originalFirst.myMatrix[5] * aSecondMatrix.myMatrix[6] +
			originalFirst.myMatrix[6] * aSecondMatrix.myMatrix[10] +
			originalFirst.myMatrix[7] * aSecondMatrix.myMatrix[14];
		aFirstMatrix.myMatrix[7] =
			originalFirst.myMatrix[4] * aSecondMatrix.myMatrix[3] +
			originalFirst.myMatrix[5] * aSecondMatrix.myMatrix[7] +
			originalFirst.myMatrix[6] * aSecondMatrix.myMatrix[11] +
			originalFirst.myMatrix[7] * aSecondMatrix.myMatrix[15];

		aFirstMatrix.myMatrix[8] =
			originalFirst.myMatrix[8] * aSecondMatrix.myMatrix[0] +
			originalFirst.myMatrix[9] * aSecondMatrix.myMatrix[4] +
			originalFirst.myMatrix[10] * aSecondMatrix.myMatrix[8] +
			originalFirst.myMatrix[11] * aSecondMatrix.myMatrix[12];
		aFirstMatrix.myMatrix[9] =
			originalFirst.myMatrix[8] * aSecondMatrix.myMatrix[1] +
			originalFirst.myMatrix[9] * aSecondMatrix.myMatrix[5] +
			originalFirst.myMatrix[10] * aSecondMatrix.myMatrix[9] +
			originalFirst.myMatrix[11] * aSecondMatrix.myMatrix[13];
		aFirstMatrix.myMatrix[10] =
			originalFirst.myMatrix[8] * aSecondMatrix.myMatrix[2] +
			originalFirst.myMatrix[9] * aSecondMatrix.myMatrix[6] +
			originalFirst.myMatrix[10] * aSecondMatrix.myMatrix[10] +
			originalFirst.myMatrix[11] * aSecondMatrix.myMatrix[14];
		aFirstMatrix.myMatrix[11] =
			originalFirst.myMatrix[8] * aSecondMatrix.myMatrix[3] +
			originalFirst.myMatrix[9] * aSecondMatrix.myMatrix[7] +
			originalFirst.myMatrix[10] * aSecondMatrix.myMatrix[11] +
			originalFirst.myMatrix[11] * aSecondMatrix.myMatrix[15];

		aFirstMatrix.myMatrix[12] =
			originalFirst.myMatrix[12] * aSecondMatrix.myMatrix[0] +
			originalFirst.myMatrix[13] * aSecondMatrix.myMatrix[4] +
			originalFirst.myMatrix[14] * aSecondMatrix.myMatrix[8] +
			originalFirst.myMatrix[15] * aSecondMatrix.myMatrix[12];
		aFirstMatrix.myMatrix[13] =
			originalFirst.myMatrix[12] * aSecondMatrix.myMatrix[1] +
			originalFirst.myMatrix[13] * aSecondMatrix.myMatrix[5] +
			originalFirst.myMatrix[14] * aSecondMatrix.myMatrix[9] +
			originalFirst.myMatrix[15] * aSecondMatrix.myMatrix[13];
		aFirstMatrix.myMatrix[14] =
			originalFirst.myMatrix[12] * aSecondMatrix.myMatrix[2] +
			originalFirst.myMatrix[13] * aSecondMatrix.myMatrix[6] +
			originalFirst.myMatrix[14] * aSecondMatrix.myMatrix[10] +
			originalFirst.myMatrix[15] * aSecondMatrix.myMatrix[14];
		aFirstMatrix.myMatrix[15] =
			originalFirst.myMatrix[12] * aSecondMatrix.myMatrix[3] +
			originalFirst.myMatrix[13] * aSecondMatrix.myMatrix[7] +
			originalFirst.myMatrix[14] * aSecondMatrix.myMatrix[11] +
			originalFirst.myMatrix[15] * aSecondMatrix.myMatrix[15];

		return aFirstMatrix;
	}

	template <typename T>
	Matrix44<T> operator*(const Matrix44<T>& aMatrix, float aScalar)
	{
		Matrix44<T> result(aMatrix);
		result *= aScalar;
		return result;
	}

	template <typename T>
	Matrix44<T> operator*=(Matrix44<T>& aMatrix, float aScalar)
	{
		aMatrix.myMatrix[0] *= aScalar;
		aMatrix.myMatrix[1] *= aScalar;
		aMatrix.myMatrix[2] *= aScalar;
		aMatrix.myMatrix[3] *= aScalar;
			 
		aMatrix.myMatrix[4] *= aScalar;
		aMatrix.myMatrix[5] *= aScalar;
		aMatrix.myMatrix[6] *= aScalar;
		aMatrix.myMatrix[7] *= aScalar;
				 
		aMatrix.myMatrix[8] *= aScalar;
		aMatrix.myMatrix[9] *= aScalar;
		aMatrix.myMatrix[10] *= aScalar;
		aMatrix.myMatrix[11] *= aScalar;
		  
		aMatrix.myMatrix[12] *= aScalar;
		aMatrix.myMatrix[13] *= aScalar;
		aMatrix.myMatrix[14] *= aScalar;
		aMatrix.myMatrix[15] *= aScalar;

		return aMatrix;
	}

	template <typename T>
	Matrix44<T> operator+(const Matrix44<T>& aMatrix, float aScalar)
	{
		Matrix44<T> result(aMatrix);
		result += aScalar;
		return result;
	}

	template <typename T>
	Matrix44<T> operator+=(Matrix44<T>& aMatrix, float aScalar)
	{
		aMatrix.myMatrix[0] += aScalar;
		aMatrix.myMatrix[1] += aScalar;
		aMatrix.myMatrix[2] += aScalar;
		aMatrix.myMatrix[3] += aScalar;
							
		aMatrix.myMatrix[4] += aScalar;
		aMatrix.myMatrix[5] += aScalar;
		aMatrix.myMatrix[6] += aScalar;
		aMatrix.myMatrix[7] += aScalar;

		aMatrix.myMatrix[8] += aScalar;
		aMatrix.myMatrix[9] += aScalar;
		aMatrix.myMatrix[10] += aScalar;
		aMatrix.myMatrix[11] += aScalar;

		aMatrix.myMatrix[12] += aScalar;
		aMatrix.myMatrix[13] += aScalar;
		aMatrix.myMatrix[14] += aScalar;
		aMatrix.myMatrix[15] += aScalar;

		return aMatrix;
	}

	template <typename T>
	Matrix44<T> Matrix44<T>::CreateRotateAroundX(T aAngleInRadians)
	{
		Matrix44<T> M;

		M.myMatrix[5] = cos(aAngleInRadians);
		M.myMatrix[6] = sin(aAngleInRadians);
		M.myMatrix[9] = -sin(aAngleInRadians);
		M.myMatrix[10] = cos(aAngleInRadians);

		return M;
	}

	template <typename T>
	Matrix44<T> Matrix44<T>::CreateRotateAroundY(T aAngleInRadians)
	{
		Matrix44<T> M;

		M.myMatrix[0] = cos(aAngleInRadians);
		M.myMatrix[2] = -sin(aAngleInRadians);
		M.myMatrix[8] = sin(aAngleInRadians);
		M.myMatrix[10] = cos(aAngleInRadians);

		return M;
	}

	template <typename T>
	Matrix44<T> Matrix44<T>::CreateRotateAroundZ(T aAngleInRadians)
	{
		Matrix44<T> M;

		M.myMatrix[0] = cos(aAngleInRadians);
		M.myMatrix[1] = sin(aAngleInRadians);
		M.myMatrix[4] = -sin(aAngleInRadians);
		M.myMatrix[5] = cos(aAngleInRadians);

		return M;
	}

	template<class T>
	Matrix44<T> Matrix44<T>::RotateAroundAxis(const CU::Vector3<T>& aAxis, T aAngleInRadians)
	{
		//http://paulbourke.net/geometry/rotate/
		//CU::Vector4<T> startPoint(aPoint.x, aPoint.y, aPoint.z, 1.f);
		//Matrix44<T> transform;
		//transform.SetPos(-startPoint);
		//transform.myMatrix[15] = 1;
		//
		//Matrix44<T> TInverse = CU::InverseSimple(transform);



		//CU::Vector3<T> U = CU::GetNormalized(aAxis);
		//if (U.x < 0.1f && U.y < 0.1f && U.z < 0.1f)
		//{
		//	return CU::Matrix44<float>();
		//}
		//
		//float a = U.x;
		//float b = U.y;
		//float c = U.z;
		//float d = sqrtf(b*b + c*c);
		//
		//Matrix44<T> Rx;
		//if (d != 0)
		//{
		//	Rx.myMatrix[5] = c / d;
		//	Rx.myMatrix[6] = (-b) / d;
		//	Rx.myMatrix[9] = b / d;
		//	Rx.myMatrix[10] = c / d;
		//}
		//
		//Matrix44<T> RxInverse = CU::InverseSimple(Rx);
		//
		//
		//Matrix44<T> Ry;
		//Ry.myMatrix[0] = d;
		//Ry.myMatrix[2] = -a;
		//Ry.myMatrix[8] = a;
		//Ry.myMatrix[10] = d;
		//
		//Matrix44<T> RyInverse = CU::InverseSimple(Ry);
		//
		//
		//Matrix44<T> Rz;
		//Rz.myMatrix[0] = cos(aAngleInRadians);
		//Rz.myMatrix[1] = -sin(aAngleInRadians);
		//Rz.myMatrix[4] = sin(aAngleInRadians);
		//Rz.myMatrix[5] = cos(aAngleInRadians);
		//
		//
		////CU::Matrix44<T> finalMatrix = TInverse * RxInverse * RyInverse * Rz * Ry * Rx * transform;
		//CU::Matrix44<T> finalMatrix =  RxInverse * RyInverse * Rz * Ry * Rx ;


		CU::Matrix33<T> A;
		A.myMatrix[0] = 0.f;
		A.myMatrix[1] = -aAxis.z;
		A.myMatrix[2] = aAxis.y;
		
		A.myMatrix[3] = aAxis.z;
		A.myMatrix[4] = 0.f;
		A.myMatrix[5] = -aAxis.x;
		
		A.myMatrix[6] = -aAxis.y;
		A.myMatrix[7] = aAxis.x;
		A.myMatrix[8] = 0.f;
		

		CU::Matrix33<T> A2 = A * A;

		float angleSin = sinf(aAngleInRadians);
		float anglecos = (1 - cosf(aAngleInRadians));

		CU::Matrix33<T> R;

		CU::Matrix33<T> idenity;
		idenity += angleSin;

		R = idenity * A;
		R += A2 * anglecos;


		CU::Matrix44<T> result;
		result.myMatrix[0] = R.myMatrix[0];
		result.myMatrix[1] = R.myMatrix[1];
		result.myMatrix[2] = R.myMatrix[2];
		result.myMatrix[4] = R.myMatrix[3];
		result.myMatrix[5] = R.myMatrix[4];
		result.myMatrix[6] = R.myMatrix[5];
		result.myMatrix[8] = R.myMatrix[6];
		result.myMatrix[9] = R.myMatrix[7];
		result.myMatrix[10] = R.myMatrix[8];

		return result;
	}

	template<class T>
	Matrix44<T> Matrix44<T>::CreateReflectionMatrixAboutAxis(CU::Vector3<T> aReflectionVector)
	{
		Matrix44<T> reflectionMatrix;
		reflectionMatrix.myMatrix[0] = 1 - 2 * (aReflectionVector.x*aReflectionVector.x);
		reflectionMatrix.myMatrix[1] = -2 * (aReflectionVector.x*aReflectionVector.y);
		reflectionMatrix.myMatrix[2] = -2 * (aReflectionVector.x*aReflectionVector.z);

		reflectionMatrix.myMatrix[4] = -2 * (aReflectionVector.y*aReflectionVector.x);
		reflectionMatrix.myMatrix[5] = 1 - 2 * (aReflectionVector.y*aReflectionVector.y);
		reflectionMatrix.myMatrix[6] = -2 * (aReflectionVector.y*aReflectionVector.z);

		reflectionMatrix.myMatrix[8] = -2 * (aReflectionVector.z*aReflectionVector.x);
		reflectionMatrix.myMatrix[9] = -2 * (aReflectionVector.z*aReflectionVector.y);
		reflectionMatrix.myMatrix[10] = 1 - 2 * (aReflectionVector.z*aReflectionVector.z);
		return reflectionMatrix;
	};

	template <typename T>
	Matrix44<T> Transpose(const Matrix44<T>& aMatrix)
	{
		Matrix44<T> result(aMatrix);

		result.myMatrix[1] = aMatrix.myMatrix[4];
		result.myMatrix[2] = aMatrix.myMatrix[8];
		result.myMatrix[3] = aMatrix.myMatrix[12];
		result.myMatrix[7] = aMatrix.myMatrix[13];
		result.myMatrix[11] = aMatrix.myMatrix[14];
		result.myMatrix[6] = aMatrix.myMatrix[9];

		result.myMatrix[4] = aMatrix.myMatrix[1];
		result.myMatrix[8] = aMatrix.myMatrix[2];
		result.myMatrix[12] = aMatrix.myMatrix[3];
		result.myMatrix[13] = aMatrix.myMatrix[7];
		result.myMatrix[14] = aMatrix.myMatrix[11];
		result.myMatrix[9] = aMatrix.myMatrix[6];

		return result;
	}

	template <typename T>
	Matrix44<T> InverseSimple(const Matrix44<T>& aMatrix)
	{
		CommonUtilities::Matrix44<float> inverse(aMatrix);

		CommonUtilities::Vector4<float> translation = inverse.GetPos4();
		inverse.SetPos(CommonUtilities::Vector4<float>(0, 0, 0, 1.f));
		translation *= -1.f;
		translation.w = 1.f;
		inverse = CommonUtilities::Transpose(inverse);
		translation = translation * inverse;

		inverse.SetPos(translation);
		return inverse;
	}

	template <typename T>
	void Matrix44<T>::SetPos(const CommonUtilities::Vector3<T>& aPos)
	{
		myMatrix[12] = aPos.x;
		myMatrix[13] = aPos.y;
		myMatrix[14] = aPos.z;
	}

	template <typename T>
	void Matrix44<T>::SetPos(const CommonUtilities::Vector4<T>& aPos)
	{
		myMatrix[12] = aPos.x;
		myMatrix[13] = aPos.y;
		myMatrix[14] = aPos.z;
		myMatrix[15] = aPos.w;
	}

	template <typename T>
	CommonUtilities::Vector3<T> Matrix44<T>::GetPos() const
	{
		return CommonUtilities::Vector3<T>(myMatrix[12], myMatrix[13], myMatrix[14]);
	}

	template <typename T>
	CommonUtilities::Vector4<T> Matrix44<T>::GetPos4() const
	{
		return CommonUtilities::Vector4<T>(myMatrix[12], myMatrix[13], myMatrix[14], myMatrix[15]);
	}

	template <typename T>
	CommonUtilities::Vector3<T> Matrix44<T>::GetForward() const
	{
		return CommonUtilities::Vector3<T>(myMatrix[8], myMatrix[9], myMatrix[10]);
	}

	template <typename T>
	CommonUtilities::Vector3<T> Matrix44<T>::GetUp() const
	{
		return CommonUtilities::Vector3<T>(myMatrix[4], myMatrix[5], myMatrix[6]);
	}

	template <typename T>
	CommonUtilities::Vector3<T> Matrix44<T>::GetRight() const
	{
		return CommonUtilities::Vector3<T>(myMatrix[0], myMatrix[1], myMatrix[2]);
	}

	template<typename T>
	Matrix44<T> Matrix44<T>::CreateProjectionMatrixLH(T aNearZ, T aFarZ, T aAspectRatio, T aFovAngle)
	{
		Matrix44 temp;
		T    SinFov;
		T    CosFov;
		T    Height;
		T    Width;

		SinFov = sin(0.5f * aFovAngle);
		CosFov = cos(0.5f * aFovAngle);

		Height = CosFov / SinFov;
		Width = Height / aAspectRatio;

		T scaling = aFarZ / (aFarZ - aNearZ);

		//temp.myVectorRows[0].Set(Width, 0.0f, 0.0f, 0.0f);
		//temp.myVectorRows[1].Set(0.0f, Height, 0.0f, 0.0f);
		//temp.myVectorRows[2].Set(0.0f, 0.0f, scaling, -scaling * aNearZ);
		//temp.myVectorRows[3].Set(0.0f, 0.0f, 1.0f, 0.0f);

		temp.myMatrix[0] = Width;
		temp.myMatrix[5] = Height;
		temp.myMatrix[10] = scaling;
		temp.myMatrix[11] = 1.f;
		temp.myMatrix[14] = -scaling * aNearZ;
		temp.myMatrix[15] = 0;

		return temp;
	}

	template<typename T>
	Matrix44<T> Matrix44<T>::CreateOrthogonalMatrixLH(T aWidth, T aHeight, T aNearZ, T aFarZ)
	{
		Matrix44 temp;

		temp.myMatrix[0] = 2.f / aWidth;
		temp.myMatrix[5] = 2.f / aHeight;
		temp.myMatrix[10] = 1.f / (aFarZ - aNearZ);
		temp.myMatrix[11] = -aNearZ / (aFarZ - aNearZ);
		temp.myMatrix[15] = 1.f;

		return temp;
	}
}