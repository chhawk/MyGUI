/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "TexturePropertyInitialisator.h"
#include "FactoryManager.h"
#include "SettingsManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(TexturePropertyInitialisator)

	TexturePropertyInitialisator::TexturePropertyInitialisator()
	{
	}

	TexturePropertyInitialisator::~TexturePropertyInitialisator()
	{
	}

	void TexturePropertyInitialisator::initialise(PropertyPtr _property)
	{
		if (SettingsManager::getInstance().getValue<bool>("Settings/SaveLastTexture"))
		{
			std::string texture = SettingsManager::getInstance().getValue("Resources/LastTextureName");
			_property->setValue(texture);
		}
	}

}
