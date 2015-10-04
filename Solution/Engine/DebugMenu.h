#pragma once

#include <GrowingArray.h>
#include <Vector.h>

namespace CommonUtilities
{
	class InputWrapper;
}

namespace Easy3D
{
	enum class eDebugVariableType
	{
		INT,
		FLOAT,
		BOOL,
		GROUP,
	};

	struct DebugGroup;
	struct DebugVariable
	{
		std::string myName;
		eDebugVariableType myType;
		Text* myText;

		DebugVariable* myNext = nullptr;
		DebugVariable* myParent = nullptr;

		union
		{
			int* myInt = nullptr;
			float* myFloat;
			bool* myBool;
			DebugGroup* myGroup;
		};
	};

	struct DebugGroup
	{
		DebugVariable* myFirstVar;
		DebugVariable* myLastVar;

		bool myIsExpanded;
		bool myIsClosed;
	};

	class Text;
	class DebugMenu
	{
	public:
		DebugMenu();
		~DebugMenu();

		void Render(const CU::InputWrapper& aInputWrapper);

		void StartGroup(const std::string& aString);
		void EndGroup();

		void AddVariable(const std::string& aString, int& aInt);
		void AddVariable(const std::string& aString, float& aFloat);
		void AddVariable(const std::string& aString, bool& aBool);
		void AddVariable(const std::string& aString, CU::Vector2<float>& aVector, bool aUseRGB = false);
		void AddVariable(const std::string& aString, CU::Vector3<float>& aVector, bool aUseRGB = false);
		void AddVariable(const std::string& aString, CU::Vector4<float>& aVector, bool aUseRGB = false);

	private:
		void LinkVariable(DebugVariable* aGroup, DebugVariable* aVar);
		CU::Vector4<float> HandleIteraction(const CU::InputWrapper& aInput, DebugVariable* aVar
			, const std::string& aVarText, const CU::Vector2<float>& aVarPos);

		void IntInteraction(const CU::InputWrapper& aInput, DebugVariable* aVar);
		void FloatInteraction(const CU::InputWrapper& aInput, DebugVariable* aVar);
		void BoolInteraction(const CU::InputWrapper& aInput, DebugVariable* aVar);
		void GroupInteraction(const CU::InputWrapper& aInput, DebugVariable* aVar);

		Text* myText;
		DebugVariable* myRootGroup;
		DebugVariable* myCurrentGroup;

		CU::Vector2<float> myTextScale;
	};
}

