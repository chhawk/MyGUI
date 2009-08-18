/*!
	@file
	@author		Albert Semenov
	@date		08/2009
	@module
*/
#ifndef __GRAPH_VIEW_H__
#define __GRAPH_VIEW_H__

#include <MyGUI.h>
#include "BaseGraphView.h"

namespace demo
{

	class GraphView : public wraps::BaseGraphView
	{
	public:
		GraphView() :
			wraps::BaseGraphView("GraphView.layout", nullptr)
		{
			MyGUI::CanvasPtr canvas = nullptr;
			assignWidget(canvas, "Canvas");
			wrapCanvas(canvas);

			assignWidget(mScrollView, "ScrollView");

			mScrollView->setCanvasSize(16, 16);
			canvas->setCoord(0, 0, 16, 16);

			mContexMenu = MyGUI::Gui::getInstance().createWidget<MyGUI::PopupMenu>("PopupMenu", MyGUI::IntCoord(), MyGUI::Align::Default, "Popup");
			mContexMenu->setVisible(false);
			mContexMenu->eventMenuCtrlAccept = MyGUI::newDelegate(this, &GraphView::notifyMenuCtrlAccept);

			eventChangeSize = MyGUI::newDelegate(this, &GraphView::notifyChangeSize);
			canvas->eventMouseButtonReleased = MyGUI::newDelegate(this, &GraphView::notifyMouseButtonReleased);

			notifyChangeSize(this, MyGUI::IntSize());
		}

		/** Event : MenuCtrl accept.\n
			signature : void method(wraps::BaseGraphView* _sender, const std::string& _id)
			@param _sender
			@param _id
		*/
		MyGUI::delegates::CDelegate2<BaseGraphView*, const std::string&> eventMenuCtrlAccept;

		void addMenuItem(const std::string& _name, const std::string& _id)
		{
			mContexMenu->addItem(_name, MyGUI::MenuItemType::Normal, _id);
		}

	private:
		void notifyChangeSize(wraps::BaseGraphView* _sender, MyGUI::IntSize _size)
		{
			const MyGUI::IntCoord& coord = mScrollView->getClientCoord();
			if (_size.width < coord.width) _size.width = coord.width;
			if (_size.height < coord.height) _size.height = coord.height;
			mScrollView->setCanvasSize(_size);
		}

		void notifyMouseButtonReleased(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id)
		{
			if (_id == MyGUI::MouseButton::Right && mContexMenu->getChildCount() > 0)
			{
				mContexMenu->setPosition(_left, _top);
				mContexMenu->setVisibleSmooth(true);
			}
		}

		void notifyMenuCtrlAccept(MyGUI::MenuCtrlPtr _sender, MyGUI::MenuItemPtr _item)
		{
			const std::string& id = _item->getItemId();
			eventMenuCtrlAccept(this, id);
		}

		/*virtual void initialise()
		{
			//mMainWidget->setCaption(mName);
			//assignBase(mConnectionIn1, "ConnectionIn1");
			//assignBase(mConnectionOut1, "ConnectionOut1");
			//assignBase(mConnectionIn2, "ConnectionIn2");
			//assignBase(mConnectionOut2, "ConnectionOut2");
		}

		virtual void shutdown()
		{
		}*/

	private:
		MyGUI::ScrollViewPtr mScrollView;
		MyGUI::PopupMenuPtr mContexMenu;
		/*std::string mName;
		wraps::BaseGraphConnection * mConnectionIn1;
		wraps::BaseGraphConnection * mConnectionOut1;
		wraps::BaseGraphConnection* mConnectionIn2;
		wraps::BaseGraphConnection* mConnectionOut2;*/


	};

} // namespace demo

#endif // __GRAPH_VIEW_H__