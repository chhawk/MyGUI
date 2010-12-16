/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#include "Precompiled.h"
#include "Localise.h"
#include "PanelMainProperties.h"
#include "EditorWidgets.h"
#include "WidgetTypes.h"
#include "CommandManager.h"
#include "PropertyFieldManager.h"

namespace tools
{
	PanelMainProperties::PanelMainProperties() :
		BasePanelViewItem("PanelMainProperties.layout"),
		mButtonRelativePosition(nullptr),
		mCurrentWidget(nullptr),
		mPositionEdit(nullptr)
	{
	}

	void PanelMainProperties::initialise()
	{
		mPanelCell->setCaption(replaceTags("Main_properties"));

		assignWidget(mButtonRelativePosition, "buttonRelativePosition");
		mButtonRelativePosition->eventMouseButtonClick += MyGUI::newDelegate(this, &PanelMainProperties::notifyToggleRelativeMode);

		EditorWidgets::getInstance().eventChangeWidgetCoord += MyGUI::newDelegate(this, &PanelMainProperties::notifyPropertyChangeCoord);

		CommandManager::getInstance().registerCommand("Command_ToggleRelativeMode", MyGUI::newDelegate(this, &PanelMainProperties::commandToggleRelativeMode));
	}

	void PanelMainProperties::shutdown()
	{
		destroyPropertyFields();
		EditorWidgets::getInstance().eventChangeWidgetCoord -= MyGUI::newDelegate(this, &PanelMainProperties::notifyPropertyChangeCoord);
	}

	void PanelMainProperties::notifyPropertyChangeCoord(MyGUI::Widget* _widget, const MyGUI::IntCoord& _coordValue, const std::string& _owner)
	{
		if (_owner == "PropertiesPanelView" || _widget != mCurrentWidget)
			return;

		updatePositionCaption();
	}

	void PanelMainProperties::notifyToggleRelativeMode(MyGUI::Widget* _sender)
	{
		if (mCurrentWidget)
		{
			WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);
			if (widgetContainer->relative_mode)
				mButtonRelativePosition->setCaption(replaceTags("to_percents"));
			else
				mButtonRelativePosition->setCaption(replaceTags("to_pixels"));
			widgetContainer->relative_mode = !widgetContainer->relative_mode;
		}

		updatePositionCaption();
	}

	void PanelMainProperties::updatePositionCaption()
	{
		if (mCurrentWidget)
		{
			WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);

			if (mPositionEdit != nullptr)
				mPositionEdit->setCaption(widgetContainer->position());
		}
	}

	void PanelMainProperties::update(MyGUI::Widget* _currentWidget)
	{
		destroyPropertyFields();

		mCurrentWidget = _currentWidget;
		if (mCurrentWidget == nullptr)
			return;

		WidgetStyle* widgetType = WidgetTypes::getInstance().findWidgetStyle(mCurrentWidget->getTypeName());
		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);

		PropertyField* field = PropertyFieldManager::getInstance().createPropertyField(mWidgetClient, "Name", widgetContainer->name, "Name", _currentWidget);
		mFields.push_back(field);

		if (widgetType->resizeable)
		{
			// update caption of LayoutEditor_buttonRelativePosition
			mButtonRelativePosition->setVisible(true);
			if (widgetContainer->relative_mode)
				mButtonRelativePosition->setCaption(replaceTags("to_pixels"));
			else
				mButtonRelativePosition->setCaption(replaceTags("to_percents"));

			PropertyField* field = PropertyFieldManager::getInstance().createPropertyField(mWidgetClient, "Position", widgetContainer->position(), "Position", _currentWidget);
			mFields.push_back(field);

			mPositionEdit = field->getField();
		}
		else
		{
			mButtonRelativePosition->setVisible(false);
		}

		field = PropertyFieldManager::getInstance().createPropertyField(mWidgetClient, "Type", widgetContainer->type, "Type", _currentWidget);
		mFields.push_back(field);

		field = PropertyFieldManager::getInstance().createPropertyField(mWidgetClient, "Align", widgetContainer->align, "Align", _currentWidget);
		mFields.push_back(field);

		if (mCurrentWidget->isRootWidget())
		{
			PropertyField* field = PropertyFieldManager::getInstance().createPropertyField(mWidgetClient, "Layer", widgetContainer->getLayerName(), "Layer", _currentWidget);
			mFields.push_back(field);
		}

		field = PropertyFieldManager::getInstance().createPropertyField(mWidgetClient, "Skin", widgetContainer->skin, "Skin", _currentWidget);
		mFields.push_back(field);

		mWidgetClient->_forcePeek(mButtonRelativePosition);

		updateSize();
	}

	void PanelMainProperties::updateSize()
	{
		int height = 0;

		for (VectorPropertyField::iterator item = mFields.begin(); item != mFields.end(); ++ item)
		{
			MyGUI::IntSize size = (*item)->getContentSize();
			(*item)->setCoord(MyGUI::IntCoord(0, height, mMainWidget->getWidth(), size.height));
			height += size.height;
		}

		mPanelCell->setClientHeight(height);
	}

	void PanelMainProperties::commandToggleRelativeMode(const MyGUI::UString& _commandName, bool& _result)
	{
		notifyToggleRelativeMode();

		_result = true;
	}

	void PanelMainProperties::destroyPropertyFields()
	{
		for (VectorPropertyField::iterator item = mFields.begin(); item != mFields.end(); ++ item)
			delete (*item);
		mFields.clear();
	}

} // namespace tools
