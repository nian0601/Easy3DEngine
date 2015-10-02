#pragma once
#include <GrowingArray.h>
#include <Vector.h>

namespace Prism
{
	struct Vertex
	{
		CU::Vector3<float> myPosition;
		CU::Vector3<float> myNormal;
		CU::Vector4<float> myColor;
	};
	struct MeshData
	{
		CU::GrowingArray<Vertex> myVertices;
		CU::GrowingArray<int> myIndices;
	};

	class GeometryGenerator
	{
	public:
		static void CreateGrid(const float aWidth, const float aDepth, const unsigned int aM, const unsigned int aN, MeshData& aOutputMeshData);

	private:
		static void GenerateHeightAndColor(Vertex& aVertex);
	};
}