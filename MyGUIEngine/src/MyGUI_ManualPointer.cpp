/*!
	@file
	@author		Albert Semenov
	@date		06/2009
	@module
*/
/*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_ManualPointer.h"
#include "MyGUI_StaticImage.h"
#include "MyGUI_CoordConverter.h"
#include "MyGUI_TextureManager.h"

namespace MyGUI
{

	/*virtual*/ void ManualPointer::deserialization(xml::ElementPtr _node, Version _version)
	{
		IntCoord coord;

		// ����� ����� � ��������, �������� ����
		xml::ElementEnumerator info = _node->getElementEnumerator();
		while (info.next("Property"))
		{
			const std::string& key = info->findAttribute("key");
			const std::string& value = info->findAttribute("value");

			if (key == "point") mPoint = IntPoint::parse(value);
			else if (key == "size") mSize = IntSize::parse(value);
			else if (key == "texture") mTexture = value;
			else if (key == "coord") coord = IntCoord::parse(value);
		}

		mOffset = CoordConverter::convertTextureCoord(
			coord,
			TextureManager::getInstance().getTextureSize(mTexture));
	}

	/*virtual*/ void ManualPointer::setImage(StaticImagePtr _image)
	{
		_image->deleteAllItems();
		_image->_setTextureName(mTexture);
		_image->_setUVSet(mOffset);
	}

	/*virtual*/ void ManualPointer::setPosition(StaticImagePtr _image, const IntPoint& _point)
	{
		_image->setCoord(_point.left - mPoint.left, _point.top - mPoint.top, mSize.width, mSize.height);
	}

} // namespace MyGUI