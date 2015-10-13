#pragma once

#include <Vector.h>

struct UpdateVelocityNote
{
	UpdateVelocityNote(const CU::Vector3<float>& aDirection);
	CU::Vector3<float> myDirection;
};

inline UpdateVelocityNote::UpdateVelocityNote(const CU::Vector3<float>& aDirection)
	: myDirection(aDirection)
{

}