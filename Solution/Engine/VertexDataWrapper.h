#pragma once

#include "Vertices.h"

namespace Easy3D
{
	struct VertexDataWrapper
	{
		int myNumberOfVertices;
		int mySize;
		int myStride;
		VertexType myType;
		char* myVertexData;

		VertexPosColor* GetDataAsVertexPosColor();
	};
}

inline Easy3D::VertexPosColor* Easy3D::VertexDataWrapper::GetDataAsVertexPosColor()
{
	return reinterpret_cast<VertexPosColor*>(myVertexData);
}