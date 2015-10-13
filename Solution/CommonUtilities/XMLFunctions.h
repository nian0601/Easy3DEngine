#pragma once
#include "tinyxml2.h"
#include "..\DL_Debug\DL_Debug.h"
#include <algorithm>
#include <string>

namespace CommonUtilities
{
	namespace XMLFunctions
	{
		void XMLErrorCheck(const tinyxml2::XMLError anXMLError, const std::string &aMessage = "", const std::string &aFileName = "");
		tinyxml2::XMLElement* GetDocument(const std::string& aFileName, tinyxml2::XMLDocument& aDocument);
		bool XMLCompareName(const char *aFirstName, const char *aSecondName);
	}
	namespace XMLF = XMLFunctions;
}namespace CU = CommonUtilities;