/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_POPUP_MENU_H__
#define __MYGUI_POPUP_MENU_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	class _MyGUIExport PopupMenu : public Widget
	{
	public:
		struct ItemInfo
		{
			ItemInfo(ButtonPtr _button, bool _separator, PopupMenuPtr _submenu) :
				button(_button),
				separator(_separator),
				submenu(_submenu)
			{
			}

			/** Button */
			ButtonPtr button;
			/** Widget have separator after button */
			bool separator;
			/** Sub menu (or NULL if no submenu) */
			PopupMenuPtr submenu;
			/** User data */
			void * data;
		};

		typedef std::vector<ItemInfo> VectorPopupMenuItemInfo;

		// ��� ������ ��������� ������������
		friend class factory::PopupMenuFactory;

	protected:
		PopupMenu(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String & _name);
		static Ogre::String WidgetTypeName;

	public:
		//! @copydoc Widget::_getType()
		inline static const Ogre::String & _getType() {return WidgetTypeName;}
		//! @copydoc Widget::getWidgetType()
		virtual const Ogre::String & getWidgetType() { return _getType(); }

		// ������ ��� ������ �� �������
		inline ItemInfo& addItem(const Ogre::UTFString& _item, bool _submenu = false, bool _separator = false) { return insertItem(ITEM_NONE, _item, _submenu, _separator); }
		ItemInfo& insertItem(size_t _index, const Ogre::UTFString& _item, bool _submenu = false, bool _separator = false);
		void setItem(size_t _index, const Ogre::UTFString& _item, bool _submenu = false, bool _separator = false);

		void deleteItem(size_t _index);
		void deleteAllItems();

		size_t getItemCount();
		const Ogre::UTFString& getItem(size_t _index);
		ItemInfo& getItemInfo(size_t _index);

		/** Show popup menu
			@param _point where popup menu will be shown (left top corner in default case)
			@param _checkBorders Check if Popup out of screen and show it up or left from _point (or up-left)
		*/
		void showPopupMenu(const IntPoint& _point, bool _checkBorders = true);

		void hidePopupMenu(bool _hideParentPopup = true);

		// event : ����� �����, ��� ������ ����
		// signature : void method(MyGUI::WidgetPtr _sender, size_t _index)
		EventInfo_WidgetSizeT eventPopupMenuAccept;

		// event : �������� ���� ���� ������� ���� ������ ������
		// signature : void method(MyGUI::WidgetPtr _sender)
		EventInfo_WidgetVoid eventPopupMenuClose;

	private:
		void notifyMouseClick(MyGUI::WidgetPtr _sender);
		void notifyOpenSubmenu(MyGUI::WidgetPtr _sender, int _left, int _top);
		void notifyMouseReleased(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id);

		void _onKeyLostFocus(WidgetPtr _new);


		void update();
		bool isRelative(WidgetPtr _widget, bool _all = false);

	private:
		VectorPopupMenuItemInfo mItems;

		int mHeightLine;
		std::string mSkinLine;

		int mSubmenuImageSize;

	}; // class _MyGUIExport PopupMenu : public Widget

} // namespace MyGUI

#endif // __MYGUI_POPUP_MENU_H__
