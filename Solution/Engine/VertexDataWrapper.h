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
	};
}