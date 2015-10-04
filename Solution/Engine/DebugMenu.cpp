#include "stdafx.h"

#include "DebugMenu.h"
#include <Intersection.h>
#include <InputWrapper.h>
#include "Text.h"
#include <sstream>

namespace Easy3D
{
	DebugMenu::DebugMenu()
	{
		myText = new Text();
		myText->Init("Data/resources/font/font.dds");

		myCurrentGroup = nullptr;
		myGroups = new DebugGroupStructure();
		myGroups->myNext = nullptr;
		myGroups->myGroup = nullptr;
		myTextScale = { 1.f, 1.f };
		myCurrentMovingVariable = nullptr;
		AddVariable("TextScale", myTextScale);
	}

	void DebugMenu::Render(const CU::InputWrapper& aInput)
	{
		if (myCurrentMovingVariable != nullptr)
		{
			UpdateMovingVariable(aInput);
		}

		DebugGroupStructure* group = myGroups;

		while (group != nullptr)
		{
			RenderGroup(group->myGroup, group->myGroup->myPosition, aInput);
			group = group->myNext;
		}
	}

	void DebugMenu::StartGroup(const std::string& aString)
	{
		DebugVariable* newGroup = new DebugVariable();
		newGroup->myName = aString;
		newGroup->myType = eDebugVariableType::GROUP;
		newGroup->myPrev = nullptr;
		newGroup->myNext = nullptr;
		newGroup->myGroup = new DebugGroup();
		newGroup->myText = new Text();
		newGroup->myText->Init("Data/resources/font/font.dds");

		newGroup->myGroup->myIsExpanded = false;
		newGroup->myGroup->myIsClosed = false;
		newGroup->myGroup->myFirstVar = nullptr;
		newGroup->myGroup->myLastVar = nullptr;

		if (myGroups->myGroup == nullptr)
		{
			myGroups->myGroup = newGroup;
			myCurrentGroup = myGroups->myGroup;
		}

		else
		{
			LinkVariable(myCurrentGroup, newGroup);
			myCurrentGroup = newGroup;
		}
	}

	void DebugMenu::EndGroup()
	{
		myCurrentGroup->myGroup->myIsClosed = true;

		if (myCurrentGroup->myParent != nullptr)
		{
			myCurrentGroup = myCurrentGroup->myParent;
		}
	}

	void DebugMenu::AddVariable(const std::string& aString, int& aInt)
	{
		DebugVariable* var = new DebugVariable();
		var->myName = aString;
		var->myType = eDebugVariableType::INT;
		var->myInt = &aInt;
		var->myText = new Text();
		var->myText->Init("Data/resources/font/font.dds");

		if (myGroups->myGroup == nullptr)
		{
			myGroups->myGroup = var;
			myCurrentGroup = myGroups->myGroup;
		}
		else
		{
			LinkVariable(myCurrentGroup, var);
		}
	}

	void DebugMenu::AddVariable(const std::string& aString, float& aFloat)
	{
		DebugVariable* var = new DebugVariable();
		var->myName = aString;
		var->myType = eDebugVariableType::FLOAT;
		var->myFloat = &aFloat;
		var->myText = new Text();
		var->myText->Init("Data/resources/font/font.dds");

		if (myGroups->myGroup == nullptr)
		{
			myGroups->myGroup = var;
			myCurrentGroup = myGroups->myGroup;
		}
		else
		{
			LinkVariable(myCurrentGroup, var);
		}
	}

	void DebugMenu::AddVariable(const std::string& aString, bool& aBool)
	{
		DebugVariable* var = new DebugVariable();
		var->myName = aString;
		var->myType = eDebugVariableType::BOOL;
		var->myBool = &aBool;
		var->myText = new Text();
		var->myText->Init("Data/resources/font/font.dds");

		if (myGroups->myGroup == nullptr)
		{
			myGroups->myGroup = var;
			myCurrentGroup = myGroups->myGroup;
		}
		else
		{
			LinkVariable(myCurrentGroup, var);
		}
	}

	void DebugMenu::AddVariable(const std::string& aString, CU::Vector2<float>& aVector, bool aUseRGB)
	{
		StartGroup(aString);

		if (aUseRGB == true)
		{
			AddVariable("R", aVector.x);
			AddVariable("G", aVector.y);
		}
		else
		{
			AddVariable("X", aVector.x);
			AddVariable("Y", aVector.y);
		}

		EndGroup();
	}

	void DebugMenu::AddVariable(const std::string& aString, CU::Vector3<float>& aVector, bool aUseRGB)
	{
		StartGroup(aString);

		if (aUseRGB == true)
		{
			AddVariable("R", aVector.x);
			AddVariable("G", aVector.y);
			AddVariable("B", aVector.z);
		}
		else
		{
			AddVariable("X", aVector.x);
			AddVariable("Y", aVector.y);
			AddVariable("Z", aVector.z);
		}

		EndGroup();
	}

	void DebugMenu::AddVariable(const std::string& aString, CU::Vector4<float>& aVector, bool aUseRGB)
	{
		StartGroup(aString);

		if (aUseRGB == true)
		{
			AddVariable("R", aVector.x);
			AddVariable("G", aVector.y);
			AddVariable("B", aVector.z);
			AddVariable("A", aVector.w);
		}
		else
		{
			AddVariable("X", aVector.x);
			AddVariable("Y", aVector.y);
			AddVariable("Z", aVector.z);
			AddVariable("W", aVector.w);
		}

		EndGroup();
	}

	void DebugMenu::AddVariable(const std::string& aString, std::function<void()> aFunction)
	{
		DebugVariable* var = new DebugVariable();
		var->myName = aString;
		var->myType = eDebugVariableType::FUNCTION;
		var->myFunction = aFunction;
		var->myText = new Text();
		var->myText->Init("Data/resources/font/font.dds");

		if (myGroups->myGroup == nullptr)
		{
			myGroups->myGroup = var;
			myCurrentGroup = myGroups->myGroup;
		}
		else
		{
			LinkVariable(myCurrentGroup, var);
		}
	}

	void DebugMenu::RenderGroup(DebugVariable* aGroup, const CU::Vector2<float>& aStartPosition
		, const CU::InputWrapper& aInput)
	{
		CU::Vector2<float> drawPos = aStartPosition;
		CU::Vector2<float> charSize;
		CU::Vector2<float> textSize;
		CU::Vector4<float> color;
		DebugVariable* var = aGroup;

		while (var != nullptr)
		{
			std::stringstream ss;
			switch (var->myType)
			{
			case eDebugVariableType::INT:
				ss << var->myName << ": " << std::to_string(*var->myInt) << std::endl;
				break;
			case eDebugVariableType::FLOAT:
				ss << var->myName << ": " << std::to_string(*var->myFloat) << std::endl;
				break;
			case eDebugVariableType::BOOL:
				ss << var->myName << ": " << (*var->myFloat ? "true" : "false") << std::endl;
				break;
			case eDebugVariableType::GROUP:
				ss << (var->myGroup->myIsExpanded ? "- " : "+ ") << var->myName << std::endl;
				break;
			case eDebugVariableType::FUNCTION:
				ss << var->myName << "()" << std::endl;
				break;
			default:
				DL_ASSERT("Tried to render a debugvariable with invalid type.");
				break;
			}

			color = HandleIteraction(aInput, var, ss.str(), drawPos);

			textSize = myText->GetTextSize(ss.str());
			charSize = var->myText->GetCharSize();
			var->myText->Render(ss.str(), drawPos, myTextScale, color);
			drawPos.y -= textSize.y;


			if (var->myType == eDebugVariableType::GROUP
				&& var->myGroup->myIsExpanded == true && var->myGroup->myFirstVar != nullptr)
			{
				var = var->myGroup->myFirstVar;
				drawPos.x += charSize.x;
			}
			else
			{
				if (var->myNext != nullptr)
				{
					var = var->myNext;
				}
				else
				{
					var = var->myParent;
					drawPos.x -= charSize.x;

					while (var != nullptr)
					{
						if (var->myNext != nullptr)
						{
							var = var->myNext;
							break;
						}

						var = var->myParent;
						drawPos.x -= charSize.x;
					}
				}
			}

		}
	}

	void DebugMenu::LinkVariable(DebugVariable* aGroup, DebugVariable* aVar)
	{
		if (aGroup != nullptr && aGroup->myType == eDebugVariableType::GROUP && aGroup->myGroup->myIsClosed == false)
		{
			aVar->myParent = aGroup;
			DebugGroup* group = aGroup->myGroup;

			if (group->myFirstVar == nullptr)
			{
				aVar->myPrev = nullptr;
				aVar->myNext = nullptr;
				group->myFirstVar = group->myLastVar = aVar;
			}
			else
			{
				aVar->myPrev = group->myLastVar;
				group->myLastVar->myNext = aVar;
				group->myLastVar = aVar;
			}
		}
		else
		{
			aVar->myPrev = aGroup;
			aGroup->myNext = aVar;
			myCurrentGroup = aVar;
		}
	}

	CU::Vector4<float> DebugMenu::HandleIteraction(const CU::InputWrapper& aInput, DebugVariable* aVar
		, const std::string& aVarText, const CU::Vector2<float>& aVarPos)
	{
		CU::Vector2<float> topLeft = { aVarPos.x, -aVarPos.y };
		CU::Vector2<float> botRight = topLeft + myText->GetTextSize(aVarText);

		if (CU::Intersection::PointVsRect(aInput.GetMousePosition(), topLeft, botRight))
		{
			switch (aVar->myType)
			{
			case eDebugVariableType::INT:
				IntInteraction(aInput, aVar);
				break;
			case eDebugVariableType::FLOAT:
				FloatInteraction(aInput, aVar);
				break;
			case eDebugVariableType::BOOL:
				BoolInteraction(aInput, aVar);
				break;
			case eDebugVariableType::GROUP:
				GroupInteraction(aInput, aVar);
				break;
			case eDebugVariableType::FUNCTION:
				FunctionInteraction(aInput, aVar);
				break;
			default:
				break;
			}

			return{ 1.f, 0.f, 0.f, 1.f };
		}

		return{ 1.f, 1.f, 1.f, 1.f };
	}


	void DebugMenu::IntInteraction(const CU::InputWrapper& aInput, DebugVariable* aVar)
	{
		if (aInput.GetMouseDZ() > 0)
		{
			++*aVar->myInt;
		}
		else if (aInput.GetMouseDZ() < 0)
		{
			--*aVar->myInt;
		}
	}

	void DebugMenu::FloatInteraction(const CU::InputWrapper& aInput, DebugVariable* aVar)
	{
		float amount = 0.1f;

		if (aInput.KeyIsPressed(DIK_LSHIFT))
		{
			amount = 0.5f;
		}
		else if (aInput.KeyIsPressed(DIK_LCONTROL))
		{
			amount = 10.f;
		}

		if (aInput.GetMouseDZ() > 0)
		{
			*aVar->myFloat += amount;
		}
		else if (aInput.GetMouseDZ() < 0)
		{
			*aVar->myFloat -= amount;
		}
	}

	void DebugMenu::BoolInteraction(const CU::InputWrapper& aInput, DebugVariable* aVar)
	{
		if (aInput.MouseDown(0))
		{
			*aVar->myBool = !*aVar->myBool;
		}
	}

	void DebugMenu::GroupInteraction(const CU::InputWrapper& aInput, DebugVariable* aVar)
	{
		if (aInput.MouseDown(0))
		{
			if (aInput.KeyIsPressed(DIK_LSHIFT))
			{
				DetatchVariable(aVar);
			}
			else
			{
				aVar->myGroup->myIsExpanded = !aVar->myGroup->myIsExpanded;
			}
		}
	}

	void DebugMenu::FunctionInteraction(const CU::InputWrapper& aInput, DebugVariable* aVar)
	{
		if (aInput.MouseDown(0))
		{
			aVar->myFunction();
		}
	}

	void DebugMenu::DetatchVariable(DebugVariable* aVariable)
	{
		DebugVariable* prev = aVariable->myPrev;
		DebugVariable* next = aVariable->myNext;

		if (prev != nullptr)
		{
			prev->myNext = next;
		}
		else
		{
			DebugVariable* parent = aVariable->myParent;
			if (parent != nullptr && parent->myType == eDebugVariableType::GROUP)
			{
				parent->myGroup->myFirstVar = next;
			}
		}
		if (next != nullptr)
		{
			next->myPrev = prev;
		}

		aVariable->myPosition = { 400.f, -100.f };
		aVariable->myNext = nullptr;
		aVariable->myPrev = nullptr;
		aVariable->myParent = nullptr;

		DebugGroupStructure* newGroup = new DebugGroupStructure();
		newGroup->myNext = nullptr;
		newGroup->myGroup = aVariable;


		DebugGroupStructure* parent = myGroups;
		while (parent->myNext != nullptr)
		{
			parent = parent->myNext;
		}
		parent->myNext = newGroup;

			myCurrentMovingVariable = aVariable;
	}

	void DebugMenu::UpdateMovingVariable(const CU::InputWrapper& aInput)
	{
		if (aInput.MouseIsPressed(0))
		{
			myCurrentMovingVariable->myPosition = aInput.GetMousePosition();
			myCurrentMovingVariable->myPosition.y *= -1.f;
		}
		else
		{
			myCurrentMovingVariable = nullptr;
		}
		
	}

}
