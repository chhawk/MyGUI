/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/
#ifndef __WIDGETS_WINDOW_H__
#define __WIDGETS_WINDOW_H__

#include "BaseLayout/BaseLayout.h"
#include "EditorToolTip.h"

namespace tools
{
	class WidgetsWindow :
		public wraps::BaseLayout
	{
	public:
		WidgetsWindow();
		virtual ~WidgetsWindow();

		void initialise();

		void clearAllSheets();

		void clearNewWidget();
		void startNewWidget(int _x1, int _y1, MyGUI::MouseButton _id);
		void createNewWidget(int _x2, int _y2);
		void finishNewWidget(int _x2, int _y2);

		int getCreatingStatus() { return creating_status; }

		MyGUI::Widget* getMainWidget() { return mMainWidget; }

	private:
		void notifyChangeSelectedWidget(MyGUI::Widget* _current_widget);
		void notifyToolTip(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo & _info);
		void notifySelectWidgetType(MyGUI::Widget* _sender);
		void notifySelectWidgetTypeDoubleclick(MyGUI::Widget* _sender);

		void updateSize();
		void setEdgeHideController();

	private:
		MyGUI::Tab* mTabSkins;

		int widgetsButtonWidth;
		int widgetsButtonHeight;
		int widgetsButtonsInOneLine;
		std::string skinSheetName;

		MyGUI::Widget* current_widget;

		EditorToolTip*  mToolTip;

		// info for new widget
		int x1, y1, x2, y2;
		// 0 - none, 1 - mouse pressed (prepare), 2 - mouse moved (widget created)
		int creating_status;

		std::string new_widget_type;
		std::string new_widget_skin;
		int mMaxLines;
	};

} // namespace tools

#endif // __WIDGETS_WINDOW_H__
