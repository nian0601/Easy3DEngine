#pragma once

#include <GrowingArray.h>
#include <Vector.h>
#include <functional>

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
		FUNCTION,
	};

	struct DebugGroup;
	struct DebugGroupStructure;
	struct DebugVariable
	{
		std::string myName;
		eDebugVariableType myType;
		CU::Vector2<float> myPosition;

		DebugVariable* myPrev = nullptr;
		DebugVariable* myNext = nullptr;
		DebugVariable* myParent = nullptr;
		DebugGroupStructure* mySuperGroup = nullptr;

		union
		{
			int* myInt = nullptr;
			float* myFloat;
			bool* myBool;
			DebugGroup* myGroup;
			
		};

		std::function<void()> myFunction;
	};

	struct DebugGroup
	{
		DebugVariable* myFirstVar;
		DebugVariable* myLastVar;

		bool myIsExpanded;
		bool myIsClosed;
	};

	struct DebugGroupStructure
	{
		DebugVariable* myGroup = nullptr;
		DebugGroupStructure* myNext = nullptr;
	};

	class Text;
	class DebugMenu
	{
	public:
		DebugMenu();
		~DebugMenu();

		void Render(const CU::InputWrapper& aInput);

		void StartGroup(const std::string& aString);
		void EndGroup();

		void AddVariable(const std::string& aString, int& aInt);
		void AddVariable(const std::string& aString, float& aFloat);
		void AddVariable(const std::string& aString, bool& aBool);
		void AddVariable(const std::string& aString, CU::Vector2<float>& aVector, bool aUseRGB = false);
		void AddVariable(const std::string& aString, CU::Vector3<float>& aVector, bool aUseRGB = false);
		void AddVariable(const std::string& aString, CU::Vector4<float>& aVector, bool aUseRGB = false);
		void AddVariable(const std::string& aString, std::function<void()> aFunction);

	private:
		void RenderGroup(DebugVariable* aGroup, const CU::Vector2<float>& aStartPosition
			, const CU::InputWrapper& aInput);

		void LinkVariable(DebugVariable* aGroup, DebugVariable* aVar);
		CU::Vector4<float> HandleIteraction(const CU::InputWrapper& aInput, DebugVariable* aVar
			, const std::string& aVarText, const CU::Vector2<float>& aVarPos);

		int GetNextVar(DebugVariable* &aVar);

		void IntInteraction(const CU::InputWrapper& aInput, DebugVariable* aVar);
		void FloatInteraction(const CU::InputWrapper& aInput, DebugVariable* aVar);
		void BoolInteraction(const CU::InputWrapper& aInput, DebugVariable* aVar);
		void GroupInteraction(const CU::InputWrapper& aInput, DebugVariable* aVar);
		void FunctionInteraction(const CU::InputWrapper& aInput, DebugVariable* aVar);

		void DeAttachVariable(DebugVariable* aVariable);
		void UpdateMovingVariable(const CU::InputWrapper& aInput);
		void DeleteVariable(DebugVariable* aVar);

		Text* myText;
		DebugVariable* myCurrentGroup;
		DebugGroupStructure* myGroups;

		DebugVariable* myCurrentMovingVariable;


		CU::Vector2<float> myTextScale;
	};
}

