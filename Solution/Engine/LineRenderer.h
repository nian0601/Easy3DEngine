#pragma once

#define MAX_NR_OF_LINES 1024

struct ID3D11InputLayout;

namespace Easy3D
{
	class Camera;
	class Effect3D;
	struct VertexBufferWrapper;

	class LineRenderer
	{
	public:
		LineRenderer();
		~LineRenderer();

		void AddLine(const CU::Vector3<float>& aStartPosition, const CU::Vector3<float>& aEndPosition
			, int aPrecision = 5, const CU::Vector4<float>& aColor = { 1.f, 1.f, 1.f, 1.f });
		void Render(const Camera& aCamera);

	private:
		struct Point
		{
			CU::Vector4<float> myPosition;
			CU::Vector4<float> myColor;
		};

		void CreateVertexBuffer();
		void CreateInputLayout();
		void UpdateVertexBuffer();

		VertexBufferWrapper* myVertexBuffer;
		CU::GrowingArray<Point> myPoints;
		Effect3D* myEffect;
		ID3D11InputLayout* myInputLayout;
	};
}