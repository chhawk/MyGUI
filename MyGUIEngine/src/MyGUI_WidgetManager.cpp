/*!
	@file
	@author		Albert Semenov
	@date		11/2007
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
#include "MyGUI_Gui.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_Widget.h"
#include "MyGUI_IWidgetFactory.h"
#include "MyGUI_FactoryManager.h"

#include "MyGUI_Button.h"
#include "MyGUI_Canvas.h"
#include "MyGUI_ComboBox.h"
#include "MyGUI_DDContainer.h"
#include "MyGUI_Edit.h"
#include "MyGUI_HScroll.h"
#include "MyGUI_ItemBox.h"
#include "MyGUI_List.h"
#include "MyGUI_ListBox.h"
#include "MyGUI_ListCtrl.h"
#include "MyGUI_MenuBar.h"
#include "MyGUI_MenuCtrl.h"
#include "MyGUI_MenuItem.h"
#include "MyGUI_Message.h"
#include "MyGUI_MultiList.h"
#include "MyGUI_PopupMenu.h"
#include "MyGUI_Progress.h"
#include "MyGUI_ScrollView.h"
#include "MyGUI_StaticImage.h"
#include "MyGUI_StaticText.h"
#include "MyGUI_Tab.h"
#include "MyGUI_TabItem.h"
#include "MyGUI_VScroll.h"
#include "MyGUI_Widget.h"
#include "MyGUI_Window.h"

namespace MyGUI
{

	template <> const char* Singleton<WidgetManager>::mClassTypeName("WidgetManager");

	WidgetManager::WidgetManager() :
		mIsInitialise(false)
	{
	}

	void WidgetManager::initialise()
	{
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

		FactoryManager& factory = FactoryManager::getInstance();

		factory.registerFactory<Button>("Widget");
		factory.registerFactory<Canvas>("Widget");
		factory.registerFactory<ComboBox>("Widget");
		factory.registerFactory<DDContainer>("Widget");
		factory.registerFactory<Edit>("Widget");
		factory.registerFactory<HScroll>("Widget");
		factory.registerFactory<ItemBox>("Widget");
		factory.registerFactory<List>("Widget");
		factory.registerFactory<ListBox>("Widget");
		factory.registerFactory<ListCtrl>("Widget");
		factory.registerFactory<MenuBar>("Widget");
		factory.registerFactory<MenuCtrl>("Widget");
		factory.registerFactory<MenuItem>("Widget");
		factory.registerFactory<Message>("Widget");
		factory.registerFactory<MultiList>("Widget");
		factory.registerFactory<PopupMenu>("Widget");
		factory.registerFactory<Progress>("Widget");
		factory.registerFactory<ScrollView>("Widget");
		factory.registerFactory<StaticImage>("Widget");
		factory.registerFactory<StaticText>("Widget");
		factory.registerFactory<Tab>("Widget");
		factory.registerFactory<TabItem>("Widget");
		factory.registerFactory<VScroll>("Widget");
		factory.registerFactory<Widget>("Widget");
		factory.registerFactory<Window>("Widget");

#ifndef MYGUI_DONT_USE_OBSOLETE
		factory.registerFactory<RenderBox>("Widget");
		factory.registerFactory<Sheet>("Widget");
#endif // MYGUI_DONT_USE_OBSOLETE

		Gui::getInstance().eventFrameStart += newDelegate(this, &WidgetManager::notifyEventFrameStart);

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void WidgetManager::shutdown()
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

		Gui::getInstance().eventFrameStart -= newDelegate(this, &WidgetManager::notifyEventFrameStart);
		_deleteDelayWidgets();

		//mFactoryList.clear();
		mDelegates.clear();
		mVectorIUnlinkWidget.clear();

		FactoryManager::getInstance().unregisterFactory("Widget");

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	Widget* WidgetManager::createWidget(WidgetStyle _style, const std::string& _type, const std::string& _skin, const IntCoord& _coord, /*Align _align, */Widget* _parent, ICroppedRectangle * _cropeedParent, const std::string& _name)
	{
		IObject* object = FactoryManager::getInstance().createObject("Widget", _type);
		if (object != nullptr)
		{
			Widget* widget = object->castType<Widget>();
			ResourceSkin* skin = SkinManager::getInstance().getByName(_skin);
			widget->_initialise(_style, _coord, /*_align, */skin, _parent, _cropeedParent, _name);

			return widget;
		}

		// старый вариант создания
		/*for (SetWidgetFactory::iterator factory = mFactoryList.begin(); factory != mFactoryList.end(); ++factory)
		{
			if ((*factory)->getTypeName() == _type)
			{
				Widget* widget = (*factory)->createWidget(_style, _skin, _coord, _align, _parent, _cropeedParent, _name);
				return widget;
			}
		}*/

		MYGUI_EXCEPT("factory '" << _type << "' not found");
		return nullptr;
	}

	void WidgetManager::destroyWidget(Widget* _widget)
	{
		Gui::getInstance().destroyWidget(_widget);
	}

	void WidgetManager::destroyWidgets(const VectorWidgetPtr& _widgets)
	{
		Gui::getInstance().destroyWidgets(_widgets);
	}

	void WidgetManager::destroyWidgets(EnumeratorWidgetPtr _widgets)
	{
		Gui::getInstance().destroyWidgets(_widgets);
	}

	void WidgetManager::registerUnlinker(IUnlinkWidget * _unlink)
	{
		unregisterUnlinker(_unlink);
		mVectorIUnlinkWidget.push_back(_unlink);
	}

	void WidgetManager::unregisterUnlinker(IUnlinkWidget * _unlink)
	{
		for (size_t pos=0; pos<mVectorIUnlinkWidget.size(); pos++)
		{
			if (mVectorIUnlinkWidget[pos] == _unlink)
			{
				mVectorIUnlinkWidget[pos] = mVectorIUnlinkWidget[mVectorIUnlinkWidget.size()-1];
				mVectorIUnlinkWidget.pop_back();
				break;
			}
		}
	}

	void WidgetManager::unlinkFromUnlinkers(Widget* _widget)
	{
		for (VectorIUnlinkWidget::iterator iter = mVectorIUnlinkWidget.begin(); iter!=mVectorIUnlinkWidget.end(); ++iter)
		{
			(*iter)->_unlinkWidget(_widget);
		}
	}

	bool WidgetManager::isFactoryExist(const std::string& _type)
	{
		if (FactoryManager::getInstance().isFactoryExist("Widget", _type))
		{
			return true;
		}

		// старый вариант
		/*for (SetWidgetFactory::iterator factory = mFactoryList.begin(); factory != mFactoryList.end(); ++factory)
		{
			if ((*factory)->getTypeName() == _type)
			{
				return true;
			}
		}*/

		return false;
	}

	void WidgetManager::notifyEventFrameStart(float _time)
	{
		_deleteDelayWidgets();
	}

	void WidgetManager::_deleteWidget(Widget* _widget)
	{
		_widget->_shutdown();
		mDestroyWidgets.push_back(_widget);
	}

	void WidgetManager::_deleteDelayWidgets()
	{
		if (!mDestroyWidgets.empty())
		{
			for (VectorWidgetPtr::iterator entry=mDestroyWidgets.begin(); entry!=mDestroyWidgets.end(); ++entry)
				delete (*entry);
			mDestroyWidgets.clear();
		}
	}

#ifndef MYGUI_DONT_USE_OBSOLETE
	Widget* WidgetManager::findWidgetT(const std::string& _name, bool _throw)
	{
		return Gui::getInstance().findWidgetT(_name, _throw);
	}

	Widget* WidgetManager::findWidgetT(const std::string& _name, const std::string& _prefix, bool _throw)
	{
		return Gui::getInstance().findWidgetT(_name, _prefix, _throw);
	}

	/*void WidgetManager::registerFactory(IWidgetFactory * _factory)
	{
		mFactoryList.insert(_factory);
		MYGUI_LOG(Info, "* Register widget factory '" << _factory->getTypeName() << "'");
	}

	void WidgetManager::unregisterFactory(IWidgetFactory * _factory)
	{
		SetWidgetFactory::iterator iter = mFactoryList.find(_factory);
		if (iter != mFactoryList.end()) mFactoryList.erase(iter);
		MYGUI_LOG(Info, "* Unregister widget factory '" << _factory->getTypeName() << "'");
	}*/

	void WidgetManager::_parse(Widget* _widget, const std::string &_key, const std::string &_value)
	{
		MapDelegate::iterator iter = mDelegates.find(_key);
		if (iter == mDelegates.end())
			_widget->setProperty(_key, _value);
		else
			iter->second(_widget, _key, _value);
	}

	ParseDelegate& WidgetManager::registerDelegate(const std::string& _key)
	{
		MapDelegate::iterator iter = mDelegates.find(_key);
		MYGUI_ASSERT(iter == mDelegates.end(), "delegate with name '" << _key << "' already exist");
		return (mDelegates[_key] = ParseDelegate());
	}

	void WidgetManager::unregisterDelegate(const std::string& _key)
	{
		MapDelegate::iterator iter = mDelegates.find(_key);
		if (iter != mDelegates.end()) mDelegates.erase(iter);
	}
#endif // MYGUI_DONT_USE_OBSOLETE
} // namespace MyGUI
