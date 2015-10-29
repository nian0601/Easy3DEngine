#include "stdafx.h"

#include <d3dx11effect.h>
#include <D3DX11async.h>
#include "Effect2D.h"

namespace Easy3D
{
	bool Effect2D::Init(const std::string& aEffectFile)
	{
		BaseEffect::Init(aEffectFile);

		myPosAndScale = CU::Vector4<float>(0.f, 0.f, 1.f, 1.f);
		return true;
	}

	bool Effect2D::ReloadShader(const std::string& aFile)
	{
		BaseEffect::ReloadShader(aFile);


		myProjectionMatrixVariable = myEffect->GetVariableByName("Projection")->AsMatrix();
		if (myProjectionMatrixVariable->IsValid() == false)
		{
			DL_MESSAGE_BOX("Failed to get ProjectionMatrix", "Effect Error", MB_ICONWARNING);
			return false;
		}

		myPosAndScaleVariable = myEffect->GetVariableByName("PosAndScale")->AsVector();
		if (myPosAndScaleVariable->IsValid() == false)
		{
			myPosAndScaleVariable = nullptr;
		}

		myColorVariable = myEffect->GetVariableByName("Color")->AsVector();
		if (myColorVariable->IsValid() == false)
		{
			myColorVariable = nullptr;
		}

		return true;
	}
		
	void Effect2D::SetPosAndScale(const CU::Vector2<float>& aPos
		, const CU::Vector2<float>& aScale)
	{
		DL_ASSERT_EXP(myPosAndScaleVariable != nullptr
			, "Effect2D: Tried to SetPosAndScale but myPosAndScaleVariable is nullptr");

		myPosAndScale.x = aPos.x;
		myPosAndScale.y = aPos.y;

		myPosAndScale.z = aScale.x;
		myPosAndScale.w = aScale.y;

		myPosAndScaleVariable->SetFloatVector(&myPosAndScale.x);
	}

	void Effect2D::SetColor(const CU::Vector4<float>& aColor)
	{
		DL_ASSERT_EXP(myColorVariable != nullptr
			, "Effect2D: Tried to SetColor but myColorVariable is nullptr");

		myColorVariable->SetFloatVector(&aColor.x);
	}

	void Effect2D::SetProjectionMatrix(const CU::Matrix44<float>& aProjectionMatrix)
	{
		DL_ASSERT_EXP(myProjectionMatrixVariable != nullptr,
			"Effect2D: Tried to SetProjectionMatrix but myProjectionMatrixVariable is nullptr");

		myProjectionMatrixVariable->SetMatrix(&aProjectionMatrix.myMatrix[0]);
	}
}