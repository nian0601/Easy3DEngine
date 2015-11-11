#include "stdafx.h"
#include "FontContainer.h"
#include "Font.h"


namespace Easy3D
{
	FontContainer* FontContainer::myInstance = nullptr;

	void FontContainer::Create()
	{
		myInstance = new FontContainer();
	}

	void FontContainer::Destroy()
	{
		delete myInstance;
	}

	FontContainer* FontContainer::GetInstance()
	{
		DL_ASSERT_EXP(myInstance != nullptr, "FontContainer: myInstance is nullptr, forgot to create?");
		return myInstance;
	}

	FontContainer::~FontContainer()
	{
		for (auto it = myFonts.begin(); it != myFonts.end(); ++it)
		{
			delete it->second;
			it->second = nullptr;
		}
	}

	Font* FontContainer::GetFont(const std::string& aPath)
	{
		auto it = myFonts.find(aPath);

		if (it == myFonts.end())
		{
			LoadFont(aPath);
		}

		return myFonts[aPath];
	}

	void FontContainer::LoadFont(const std::string& aPath)
	{
		Font* newFont = new Font();
		newFont->Init(aPath.c_str());

		myFonts[aPath] = newFont;
	}
}