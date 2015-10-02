#include "stdafx.h"
#include "GeometryGenerator.h"


void Prism::GeometryGenerator::CreateGrid(const float aWidth, const float aDepth, 
		const unsigned int aM, const unsigned int aN, MeshData& aOutputMeshData)
{
	unsigned int vertexCount = aM * aN;
	unsigned int faceCount = (aM - 1) * (aN - 1) * 2;

	//Generate Vertices
	float halfWidth = aWidth / 2.f;
	float halfDepth = aDepth / 2.f;

	float dx = aWidth / (aN - 1);
	float dz = aDepth / (aM - 1);

	aOutputMeshData.myVertices.Reserve(vertexCount);
	for (unsigned int i = 0; i < aM; ++i)
	{
		float z = halfDepth - i * dz;

		for (unsigned int j = 0; j < aN; ++j)
		{
			float x = -halfWidth + j*dx;

			aOutputMeshData.myVertices[i*aN + j].myPosition = CU::Vector3<float>(x, 0.f, z);
			//aOutputMeshData.myVertices[i*aN + j].myNormal = CU::Vector3<float>(0.f, 1.f, 0.f);
			GenerateHeightAndColor(aOutputMeshData.myVertices[i*aN + j]);
			//aOutputMeshData.myVertices[i*aN + j].myNormal = CU::GetNormalized(aOutputMeshData.myVertices[i*aN + j].myPosition);
		}
	}

	//Generate Indices
	aOutputMeshData.myIndices.Reserve(faceCount * 3);
	unsigned int k = 0;
	for (unsigned int i = 0; i < aM - 1; ++i)
	{
		for (unsigned int j = 0; j < aN - 1; ++j)
		{
			aOutputMeshData.myIndices[k] = i * aN + j;
			aOutputMeshData.myIndices[k + 1] = i * aN + j + 1;
			aOutputMeshData.myIndices[k + 2] = (i + 1) * aN + j;
			aOutputMeshData.myIndices[k + 3] = (i + 1) * aN + j;
			aOutputMeshData.myIndices[k + 4] = i * aN + j + 1;
			aOutputMeshData.myIndices[k + 5] = (i + 1) * aN + j + 1;

			k += 6;
		}
	}

	for (int i = 0; i < aOutputMeshData.myIndices.Size() - 2; i += 3)
	{
		int indexOne = aOutputMeshData.myIndices[i];
		int indexTwo = aOutputMeshData.myIndices[i + 1];
		int indexThree = aOutputMeshData.myIndices[i + 2];

		CU::Vector3<float> point1 = aOutputMeshData.myVertices[indexOne].myPosition;
		CU::Vector3<float> point2 = aOutputMeshData.myVertices[indexTwo].myPosition;
		CU::Vector3<float> point3 = aOutputMeshData.myVertices[indexThree].myPosition;

		CU::Vector3<float> norm1 = CU::Cross(point2 - point1, point3 - point1);
		CU::Vector3<float> norm2 = CU::Cross(point1 - point2, point3 - point2);
		CU::Vector3<float> norm3 = CU::Cross(point1 - point3, point2 - point3);

		aOutputMeshData.myVertices[indexOne].myNormal += norm1;
		aOutputMeshData.myVertices[indexTwo].myNormal += norm2;
		aOutputMeshData.myVertices[indexThree].myNormal += norm3;

		CU::Normalize(aOutputMeshData.myVertices[indexOne].myNormal);
		CU::Normalize(aOutputMeshData.myVertices[indexTwo].myNormal);
		CU::Normalize(aOutputMeshData.myVertices[indexThree].myNormal);
	}
}

void Prism::GeometryGenerator::GenerateHeightAndColor(Vertex& aVertex)
{
	float divFactor = ((rand() % 10) / 10) + 25.f;

	float xCoef = 1.55f;
	float zCoef = 1.55f;

	float height = 0.3f * (aVertex.myPosition.z * cosf((xCoef * aVertex.myPosition.x) / divFactor) 
			+ aVertex.myPosition.x * cosf(zCoef * aVertex.myPosition.z / divFactor));

	//float divFactor = 25.f;
	//float height = 0.3f * (cosf((aVertex.myPosition.x / divFactor)) + cosf((aVertex.myPosition.z / divFactor)));
	//
	//height *= 250.f;
	aVertex.myPosition.y = height;

	if (height < -10.f)
	{
		//BeachColor
		aVertex.myColor = { 1.f, 0.96f, 0.62f, 1.0f };
	}
	else if (height < 5.f)
	{
		//Light yellow-green
		aVertex.myColor = { 0.48f, 0.77f, 0.46f, 1.0f };
	}
	else if (height < 12.f)
	{
		//Dark yellow-green
		aVertex.myColor = { 0.1f, 0.48f, 0.19f, 1.0f };
	}
	else if (height < 20.f)
	{
		//Dark brown
		aVertex.myColor = { 0.45f, 0.39f, 0.34f, 1.0f };
	}
	else
	{
		//White snow
		aVertex.myColor = { 1.f, 1.f, 1.f, 1.0f };
	}
}