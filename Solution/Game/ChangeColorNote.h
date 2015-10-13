#pragma once

#include <Vector.h>

struct ChangeColorNote
{
	ChangeColorNote(const CU::Vector4<float>& aColor);

	CU::Vector4<float> myColor;
};

inline ChangeColorNote::ChangeColorNote(const CU::Vector4<float>& aColor)
	: myColor(aColor)
{

}