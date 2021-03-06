/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "SeparatorControl.h"
#include "FactoryManager.h"
#include "SettingsManager.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(SeparatorControl)

	SeparatorControl::SeparatorControl() :
		mHorizontal(false)
	{
	}

	SeparatorControl::~SeparatorControl()
	{
		SaveSeparators();

		mMainWidget->eventChangeCoord -= MyGUI::newDelegate(this, &SeparatorControl::notifyChangeCoord);

		for (VectorWidget::const_iterator child = mChilds.begin(); child != mChilds.end(); child ++)
		{
			SeparatorData** data = (*child)->getUserData<SeparatorData*>(false);
			if (data != nullptr)
			{
				delete (*data);
				(*child)->setUserData(nullptr);
			}
		}
	}

	void SeparatorControl::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, _layoutName);

		CreateSeparators();

		LoadSeparators();

		mMainWidget->eventChangeCoord += MyGUI::newDelegate(this, &SeparatorControl::notifyChangeCoord);

		updateAllSeparators();
	}

	void SeparatorControl::CreateSeparators()
	{
		for (size_t index = 0; index < mMainWidget->getChildCount(); index ++)
		{
			MyGUI::Widget* widget = mMainWidget->getChildAt(index);
			mChilds.push_back(widget);

			std::string separator = widget->getUserString("Separator");
			if (separator == "Horizontal")
				mHorizontal = true;
		}

		if (mHorizontal)
			std::sort(mChilds.begin(), mChilds.end(), sortHorizontal);
		else
			std::sort(mChilds.begin(), mChilds.end(), sortVertical);

		MyGUI::Widget* lastPart = nullptr;
		MyGUI::Widget* lastSeparator = nullptr;
		for (VectorWidget::const_iterator child = mChilds.begin(); child != mChilds.end(); child ++)
		{
			std::string separator = (*child)->getUserString("Separator");
			if (!separator.empty())
			{
				(*child)->eventMouseButtonPressed += MyGUI::newDelegate(this, &SeparatorControl::notifyMouseButtonPressed);
				(*child)->eventMouseDrag += MyGUI::newDelegate(this, &SeparatorControl::notifyMouseDrag);

				if (lastSeparator != nullptr)
				{
					SeparatorData* data = *lastSeparator->getUserData<SeparatorData*>();
					data->NextSeparator = (*child);
				}

				SeparatorData* data = new SeparatorData();
				data->PreviousPart = lastPart;
				data->PreviousSeparator = lastSeparator;
				(*child)->setUserData(data);

				lastSeparator = (*child);
			}
			else
			{
				lastPart = (*child);
				if (lastSeparator != nullptr)
				{
					SeparatorData* data = *lastSeparator->getUserData<SeparatorData*>();
					data->NextPart = lastPart;
				}
			}
		}
	}

	void SeparatorControl::notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id != MyGUI::MouseButton::Left)
			return;

		SeparatorData* data = *_sender->getUserData<SeparatorData*>();
		data->OffsetPoint = MyGUI::IntPoint(_left, _top) - _sender->getAbsolutePosition();
	}

	void SeparatorControl::notifyMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id != MyGUI::MouseButton::Left)
			return;

		SeparatorData* data = *_sender->getUserData<SeparatorData*>();
		MyGUI::IntPoint parentAbsolutePosition = _sender->getParent()->getAbsolutePosition();
		MyGUI::IntPoint offset = MyGUI::IntPoint(_left, _top) - parentAbsolutePosition - data->OffsetPoint;

		MoveSeparator(_sender, offset);
	}

	void SeparatorControl::MoveSeparator(MyGUI::Widget* _separator, const MyGUI::IntPoint& _point, SeparatorDirection _modeDirection)
	{
		SeparatorData* data = *_separator->getUserData<SeparatorData*>();

		if (data->PreviousPart == nullptr || data->NextPart == nullptr)
			return;

		MyGUI::IntPoint point = _point;
		if (mHorizontal)
		{
			if (point.left < 0)
				point.left = 0;
			if (point.left > _separator->getParentSize().width)
				point.left = _separator->getParentSize().width;
		}
		else
		{
			if (point.top < 0)
				point.top = 0;
			if (point.top > _separator->getParentSize().height)
				point.top = _separator->getParentSize().height;
		}

		MyGUI::IntCoord previousCoord = data->PreviousPart->getCoord();
		MyGUI::IntCoord nextCoord = data->NextPart->getCoord();
		MyGUI::IntCoord separatorCoord = _separator->getCoord();

		if (mHorizontal)
		{
			previousCoord.width = point.left - previousCoord.left;
			separatorCoord.left = point.left;
			nextCoord.width = nextCoord.right() - (point.left + separatorCoord.width);
			nextCoord.left = point.left + separatorCoord.width;
		}
		else
		{
			previousCoord.height = point.top - previousCoord.top;
			separatorCoord.top = point.top;
			nextCoord.height = nextCoord.bottom() - (point.top + separatorCoord.height);
			nextCoord.top = point.top + separatorCoord.height;
		}

		data->PreviousPart->setCoord(previousCoord);
		data->NextPart->setCoord(nextCoord);
		_separator->setCoord(separatorCoord);

		int size = MyGUI::utility::parseValue<int>(data->PreviousPart->getUserString("MinSize"));

		if (mHorizontal)
		{
			if (size > previousCoord.width && _modeDirection != Next)
			{
				if (data->PreviousSeparator != nullptr)
				{
					MoveSeparator(data->PreviousSeparator, point - MyGUI::IntPoint(size + data->PreviousSeparator->getWidth(), data->PreviousSeparator->getHeight()), Previous);
				}

				previousCoord = data->PreviousPart->getCoord();
				nextCoord = data->NextPart->getCoord();
				separatorCoord = _separator->getCoord();

				int offset = size - previousCoord.width;

				previousCoord.width += offset;
				separatorCoord.left += offset;
				nextCoord.width -= offset;
				nextCoord.left += offset;

				data->PreviousPart->setCoord(previousCoord);
				data->NextPart->setCoord(nextCoord);
				_separator->setCoord(separatorCoord);
			}

			size = MyGUI::utility::parseValue<int>(data->NextPart->getUserString("MinSize"));
			if (size > nextCoord.width && _modeDirection != Previous)
			{
				if (data->NextSeparator != nullptr)
				{
					MoveSeparator(data->NextSeparator, point + MyGUI::IntPoint(size + _separator->getWidth(), _separator->getHeight()), Next);
				}

				previousCoord = data->PreviousPart->getCoord();
				nextCoord = data->NextPart->getCoord();
				separatorCoord = _separator->getCoord();

				int offset = size - nextCoord.width;

				previousCoord.width -= offset;
				separatorCoord.left -= offset;
				nextCoord.width += offset;
				nextCoord.left -= offset;

				data->PreviousPart->setCoord(previousCoord);
				data->NextPart->setCoord(nextCoord);
				_separator->setCoord(separatorCoord);
			}
		}
		else
		{
			if (size > previousCoord.height && _modeDirection != Next)
			{
				if (data->PreviousSeparator != nullptr)
				{
					MoveSeparator(data->PreviousSeparator, point - MyGUI::IntPoint(data->PreviousSeparator->getWidth(), size + data->PreviousSeparator->getHeight()), Previous);
				}

				previousCoord = data->PreviousPart->getCoord();
				nextCoord = data->NextPart->getCoord();
				separatorCoord = _separator->getCoord();

				int offset = size - previousCoord.height;

				previousCoord.height += offset;
				separatorCoord.top += offset;
				nextCoord.height -= offset;
				nextCoord.top += offset;

				data->PreviousPart->setCoord(previousCoord);
				data->NextPart->setCoord(nextCoord);
				_separator->setCoord(separatorCoord);
			}

			size = MyGUI::utility::parseValue<int>(data->NextPart->getUserString("MinSize"));
			if (size > nextCoord.height && _modeDirection != Previous)
			{
				if (data->NextSeparator != nullptr)
				{
					MoveSeparator(data->NextSeparator, point + MyGUI::IntPoint(_separator->getWidth(), size + _separator->getHeight()), Next);
				}

				previousCoord = data->PreviousPart->getCoord();
				nextCoord = data->NextPart->getCoord();
				separatorCoord = _separator->getCoord();

				int offset = size - nextCoord.height;

				previousCoord.height -= offset;
				separatorCoord.top -= offset;
				nextCoord.height += offset;
				nextCoord.top -= offset;

				data->PreviousPart->setCoord(previousCoord);
				data->NextPart->setCoord(nextCoord);
				_separator->setCoord(separatorCoord);
			}
		}
	}

	void SeparatorControl::notifyChangeCoord(MyGUI::Widget* _sender)
	{
		updateAllSeparators();
	}

	void SeparatorControl::updateAllSeparators()
	{
		for (VectorWidget::const_iterator child = mChilds.begin(); child != mChilds.end(); child ++)
		{
			SeparatorData** data = (*child)->getUserData<SeparatorData*>(false);
			if (data != nullptr)
			{
				MoveSeparator((*child), (*child)->getPosition());
			}
		}
	}

	void SeparatorControl::LoadSeparators()
	{
		mSaveAs = mMainWidget->getUserString("SaveAs");
		if (mSaveAs.empty())
			return;

		SettingsManager::VectorString values;
		values = SettingsManager::getInstance().getValueList("Controls/SeparatorControl/" + mSaveAs + ".List");
		SettingsManager::VectorString::const_iterator value = values.begin();

		for (VectorWidget::const_iterator child = mChilds.begin(); child != mChilds.end(); child ++)
		{
			SeparatorData** data = (*child)->getUserData<SeparatorData*>(false);
			if (data != nullptr)
			{
				if (value != values.end())
				{
					MyGUI::IntPoint point = MyGUI::IntPoint::parse(*value);
					if (mHorizontal)
						point.top = 0;
					else
						point.left = 0;

					(*child)->setPosition(point);

					value ++;
				}
			}
		}
	}

	void SeparatorControl::SaveSeparators()
	{
		if (mSaveAs.empty())
			return;

		SettingsManager::VectorString values;
		for (VectorWidget::const_iterator child = mChilds.begin(); child != mChilds.end(); child ++)
		{
			SeparatorData** data = (*child)->getUserData<SeparatorData*>(false);
			if (data != nullptr)
				values.push_back((*child)->getPosition().print());
		}

		SettingsManager::getInstance().setValueList("Controls/SeparatorControl/" + mSaveAs + ".List", values);
	}

}
