#pragma once
#include <GrowingArray.h>
#include <functional>

#define ADD_FUNCTION_TO_RADIAL_MENU(ELEMENT_NAME, FUNCTION, OBJECT_TO_CALL_FUNC_ON) \
	(Prism::Engine::GetInstance()->GetDebugDisplay()->GetDebugMenu()->AddFunctionCallback(ELEMENT_NAME, std::bind(&FUNCTION, OBJECT_TO_CALL_FUNC_ON)))

namespace CommonUtilities
{
	class InputWrapper;
}

namespace Prism
{
	class Text;

	class DebugMenu
	{
	public:
		DebugMenu();

		void Update(CU::InputWrapper& aInputWrapper);
		void Render();

		void AddFunctionCallback(const std::string& aMenuElementName, std::function<void()> aCallBack);

	private:
		struct MenuElement
		{
			std::string myName;
			std::function<void()> myCallBack;
		};

		CU::GrowingArray<MenuElement> myMenuElements;
		Text* myText;
		bool myIsVisible;
		CU::Vector2<float> myCenterPosition;
		int mySelectedIndex;
		float myRadius;
		float myInteractRange;
		float myThetaStep;
	};
}