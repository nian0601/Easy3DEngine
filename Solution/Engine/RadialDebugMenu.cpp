#include "stdafx.h"

#include "RadialDebugMenu.h"
#include "Engine.h"
#include "FontContainer.h"
#include <InputWrapper.h>
#include "Text.h"

namespace Easy3D
{
	RadialDebugMenu::RadialDebugMenu()
		: myIsVisible(false)
		, mySelectedIndex(-1)
		, myRadius(200.f)
		, myInteractRange(150.f)
	{
		myText = new Text();
		myText->Init("Data/resources/font/font.dds");

		myMenuElements.Init(4);
	}


	void RadialDebugMenu::Update(CU::InputWrapper& aInputWrapper)
	{
		if (aInputWrapper.MouseDown(1))
		{
			myIsVisible = true;
			myCenterPosition = aInputWrapper.GetMousePosition();
			aInputWrapper.PauseDeltaRecording();
		}
		else if (aInputWrapper.MouseUp(1))
		{
			aInputWrapper.ResumeDeltaRecording();
			myIsVisible = false;

			if (mySelectedIndex != -1)
			{
				myMenuElements[mySelectedIndex].myCallBack();
			}
		}

		if (myIsVisible == true)
		{
			if (aInputWrapper.MouseIsPressed(1))
			{
				myRadius += static_cast<float>(aInputWrapper.GetMouseDZ());
				myInteractRange += static_cast<float>(aInputWrapper.GetMouseDZ());

				CU::Vector2<float> mousePos = aInputWrapper.GetMousePosition();
				mousePos.y *= -1;

				float bestDistance = FLT_MAX;
				float currDistance = 0.f;
				mySelectedIndex = -1;
				for (int i = 0; i < myMenuElements.Size(); ++i)
				{

					CU::Vector2<float> textPosition;
					textPosition.x = (cos(myThetaStep * i) * myRadius) + myCenterPosition.x;
					textPosition.y = (sin(myThetaStep * i) * myRadius) - myCenterPosition.y;



					currDistance = CU::Length(mousePos - textPosition);

					if (currDistance <= myInteractRange && currDistance < bestDistance)
					{
						bestDistance = currDistance;
						mySelectedIndex = i;
					}
				}
			}

			if (aInputWrapper.MouseDown(0))
			{
				if (mySelectedIndex != -1)
				{
					myMenuElements[mySelectedIndex].myCallBack();
				}
			}
		}
	}

	void RadialDebugMenu::Render()
	{
		if (myIsVisible == true)
		{
			for (int i = 0; i < myMenuElements.Size(); ++i)
			{
				CU::Vector2<float> textPosition;
				CU::Vector2<float> textSize = myText->GetTextSize(myMenuElements[i].myName.c_str());
				textPosition.x = (cos(myThetaStep * i) * myRadius) + myCenterPosition.x - (textSize.x / 2.f);
				textPosition.y = (sin(myThetaStep * i) * myRadius) - myCenterPosition.y + (textSize.y / 2.f);

				CU::Vector4<float> color = { 1.f, 1.f, 1.f, 1.f };
				if (i == mySelectedIndex)
				{
					color = { 0.f, 0.f, 1.f, 1.f };
				}

				myText->Render(myMenuElements[i].myName.c_str(), { textPosition.x, textPosition.y }
				, { 1.f, 1.f, },  color);
			}
		}
	}

	void RadialDebugMenu::AddFunctionCallback(const std::string& aMenuElementName, std::function<void()> aCallBack)
	{
		MenuElement newElement;
		newElement.myName = aMenuElementName;
		newElement.myCallBack = aCallBack;

		myMenuElements.Add(newElement);

		myThetaStep = (3.14f * 2.f) / myMenuElements.Size();
	}
}