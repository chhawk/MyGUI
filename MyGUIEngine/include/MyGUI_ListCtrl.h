/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*//*
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
#ifndef __MYGUI_LIST_CTRL_H__
#define __MYGUI_LIST_CTRL_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_DDContainer.h"
#include "MyGUI_IBItemInfo.h"
#include "MyGUI_Any.h"
#include "MyGUI_EventPair.h"

namespace MyGUI
{

	typedef delegates::CDelegate2<ListCtrlPtr, WidgetPtr> EventHandle_ListCtrlPtrWidgetPtr;
	//typedef delegates::CDelegate3<ListCtrlPtr, IntCoord&, bool> EventHandle_ListCtrlPtrIntCoordRefBool;
	typedef delegates::CDelegate3<ListCtrlPtr, WidgetPtr, const IBDrawItemInfo &> EventHandle_ListCtrlPtrWidgetPtrCIBCellDrawInfoRef;
	typedef delegates::CDelegate2<ListCtrlPtr, size_t> EventHandle_ListCtrlPtrSizeT;
	typedef delegates::CDelegate2<ListCtrlPtr, const IBNotifyItemData &> EventHandle_ListCtrlPtrCIBNotifyCellDataRef;

	class MYGUI_EXPORT ListCtrl : public DDContainer
	{
		// для вызова закрытого конструктора
		friend class factory::BaseWidgetFactory<ListCtrl>;

		MYGUI_RTTI_CHILD_HEADER( ListCtrl, DDContainer );

	public:
		//------------------------------------------------------------------------------//
		// манипуляции айтемами

		//! Get number of items
		size_t getItemCount() { return mCountItems; }

		//! Insert an item into a array at a specified position
		void insertItemAt(size_t _index, Any _data = Any::Null);

		//! Add an item to the end of a array
		void addItem(Any _data = Any::Null) { insertItemAt(ITEM_NONE, _data); }

		//! Remove item at a specified position
		void removeItemAt(size_t _index);

		//! Remove all items
		void removeAllItems();

		//! Redraw at a specified position
		void redrawItemAt(size_t _index);

		//! Redraw all items
		void redrawAllItems() { _updateAllVisible(true); }


		//------------------------------------------------------------------------------//
		// манипуляции выделениями

		//! Get index of selected item (ITEM_NONE if none selected)
		size_t getIndexSelected() { return mIndexSelect; }

		//! Select specified _index
		void setIndexSelected(size_t _index);

		//! Clear item selection
		void clearIndexSelected() { setIndexSelected(ITEM_NONE); }


		//------------------------------------------------------------------------------//
		// манипуляции данными

		//! Replace an item data at a specified position
		void setItemDataAt(size_t _index, Any _data);

		//! Clear an item data at a specified position
		void clearItemDataAt(size_t _index) { setItemDataAt(_index, Any::Null); }

		//! Get item data from specified position
		template <typename ValueType>
		ValueType * getItemDataAt(size_t _index, bool _throw = true)
		{
			MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "ListCtrl::getItemDataAt");
			return mItemsInfo[_index].data.castType<ValueType>(_throw);
		}


		/** Get item index by item Widget pointer */
		size_t getIndexByWidget(WidgetPtr _widget);

		/** Get widget created for drop */
		WidgetPtr getWidgetDrag() { return mItemDrag; }

		/** Get item Widget pointer by item index if it is visible
			@note returned widget can be deleted, so this pointer
			is valid only at time when you got it and can be invalid
			next frame
		*/
		WidgetPtr getWidgetByIndex(size_t _index);

		/** Interrupt drag as if widget was dropped into empty space */
		void resetDrag() { endDrop(true); }

		//! @copydoc Widget::setPosition(const IntPoint & _point)
		virtual void setPosition(const IntPoint & _point);
		//! @copydoc Widget::setSize(const IntSize& _size)
		virtual void setSize(const IntSize & _size);
		//! @copydoc Widget::setCoord(const IntCoord & _coord)
		virtual void setCoord(const IntCoord & _coord);

		/** @copydoc Widget::setPosition(int _left, int _top) */
		void setPosition(int _left, int _top) { setPosition(IntPoint(_left, _top)); }
		/** @copydoc Widget::setSize(int _width, int _height) */
		void setSize(int _width, int _height) { setSize(IntSize(_width, _height)); }
		/** @copydoc Widget::setCoord(int _left, int _top, int _width, int _height) */
		void setCoord(int _left, int _top, int _width, int _height) { setCoord(IntCoord(_left, _top, _width, _height)); }

		/** Show VScroll when text size larger than Edit */
		//void setVisibleVScroll(bool _visible) { mShowVScroll = _visible; updateView(false); }
		/** Get Show VScroll flag */
		//bool isVisibleVScroll() { return mShowVScroll; }

		/** Show HScroll when text size larger than Edit */
		//void setVisibleHScroll(bool _visible) { mShowHScroll = _visible; updateView(false); }
		/** Get Show HScroll flag */
		//bool isVisibleHScroll() { return mShowHScroll; }

	/*event:*/
		/** Event : request for creating new item
			signature : void method(MyGUI::ListCtrlPtr _sender, MyGUI::WidgetPtr _item)
			@param _sender widget that called this event
			@param _item widget item pointer
		*/
		EventHandle_ListCtrlPtrWidgetPtr requestCreateWidgetItem;

		/** Event : request for item coordinate
			signature : void method(MyGUI::ListCtrlPtr _sender, MyGUI::IntCoord & _coord, bool _drag)
			@param _sender widget that called this event
			@param _coord write heer item coordinate
			@param _drag is this item dragging
		*/
		//EventHandle_ListCtrlPtrIntCoordRefBool requestCoordItem;

		/** Event : request for item redraw
			signature : void method(MyGUI::ListCtrlPtr _sender, MyGUI::WidgetPtr _item, const MyGUI::IBDrawItemInfo & _info)
			@param _sender widget that called this event
			@param _item widget item pointer
			@param _info item info
		*/
		EventHandle_ListCtrlPtrWidgetPtrCIBCellDrawInfoRef requestDrawItem;

		/** Event : doubleclick or enter pressed on item
			signature : void method(MyGUI::ListCtrlPtr _sender, size_t _index)
			@param _sender widget that called this event
			@param _index item index
		*/
		EventHandle_ListCtrlPtrSizeT eventSelectItemAccept;

		/** Event : position of selected item was changed
			signature : void method(MyGUI::ListCtrlPtr _sender, size_t _index)
			@param _sender widget that called this event
			@param _index item index
		*/
		EventHandle_ListCtrlPtrSizeT eventChangeItemPosition;

		/** Event : click on item
			signature : void method(MyGUI::ListCtrlPtr _sender, size_t _index)
			@param _sender widget that called this event
			@param _index item index
		*/
		EventHandle_ListCtrlPtrSizeT eventMouseItemActivate;

		/** Event : notify about event in item widget
			signature : void method(MyGUI::ListCtrlPtr _sender, const MyGUI::IBNotifyItemData & _info)
			@param _sender widget that called this event
			@param _info info about item notify
		*/
		EventHandle_ListCtrlPtrCIBNotifyCellDataRef eventNotifyItem;

	protected:
		struct ItemDataInfo
		{
			ItemDataInfo(Any _data) :
				data(_data)
			{
			}
			Any data;
			IntSize size;
		};
		typedef std::vector<ItemDataInfo> VectorItemInfo;

		ListCtrl(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string & _name);
		virtual ~ListCtrl();

		void baseChangeWidgetSkin(WidgetSkinInfoPtr _info);

		virtual void onMouseButtonPressed(int _left, int _top, MouseButton _id);
		virtual void onMouseButtonReleased(int _left, int _top, MouseButton _id);
		virtual void onMouseDrag(int _left, int _top);

		virtual void onMouseWheel(int _rel);
		virtual void onKeyLostFocus(WidgetPtr _new);
		virtual void onKeySetFocus(WidgetPtr _old);

		void notifyKeyButtonPressed(WidgetPtr _sender, KeyCode _key, Char _char);
		void notifyKeyButtonReleased(WidgetPtr _sender, KeyCode _key);
		void notifyScrollChangePosition(VScrollPtr _sender, size_t _index);
		void notifyMouseWheel(WidgetPtr _sender, int _rel);
		void notifyRootMouseChangeFocus(WidgetPtr _sender, bool _focus);
		void notifyMouseButtonDoubleClick(WidgetPtr _sender);
		void _requestGetContainer(WidgetPtr _sender, WidgetPtr & _container, size_t & _index);
		void notifyMouseDrag(WidgetPtr _sender, int _left, int _top);
		void notifyMouseButtonPressed(WidgetPtr _sender, int _left, int _top, MouseButton _id);
		void notifyMouseButtonReleased(WidgetPtr _sender, int _left, int _top, MouseButton _id);


		virtual void removeDropItems();
		virtual void updateDropItems();
		virtual void updateDropItemsState(const DDWidgetState & _state);

		// Обновляет данные о айтемах, при изменении размеров
		void updateMetrics();

		// обновляет скролл, по текущим метрикам
		void updateScroll();

		// просто обновляет все виджеты что видны
		void _updateAllVisible(bool _redraw);

		void updateFromResize(const IntSize& _size);

		// возвращает следующий айтем, если нет его, то создается
		// запросы только последовательно
		WidgetPtr getItemWidget(size_t _index);

		void _updateScrollWidget();

		void _setContainerItemInfo(size_t _index, bool _set, bool _accept);

		// сбрасываем старую подсветку
		void resetCurrentActiveItem();
		// ищет и устанавливает подсвеченный айтем
		void findCurrentActiveItem();

		// запрашиваем у конейтера айтем по позиции мыши
		virtual size_t _getContainerIndex(const IntPoint & _point);

		// сбрасывает зависимости, при любом колличественном изменении
		virtual void _resetContainer(bool _update);

	private:
		void initialiseWidgetSkin(WidgetSkinInfoPtr _info);
		void shutdownWidgetSkin();

		//void updateScroll();

	private:
		VScrollPtr mVScroll;
		HScrollPtr mHScroll;

		bool mShowHScroll;
		bool mShowVScroll;

		size_t mVRange;
		size_t mHRange;

		// текущий размер всех айтемов
		//IntSize mContentSize;

		// наши дети в строках
		VectorWidgetPtr mVectorItems;

		// размер одного айтема
		IntSize mSizeItem;

		// размерность скролла в пикселях
		int mScrollRange;
		// позиция скролла п пикселях
		int mScrollPosition;

		// колличество айтемов в одной строке
		int mCountItemInLine;
		// колличество линий
		int mCountLines;
		// колличество айтемов всего
		size_t mCountItems;
		// максимальное колличество видимых линий
		int mCountLineVisible;


		// самая верхняя строка
		int mLineTop;
		// текущее смещение верхнего элемента в пикселях
		// сколько его пикселей не видно сверху
		int mOffsetTop;

		// текущий выделенный элемент или ITEM_NONE
		size_t mIndexSelect;
		// подсвеченный элемент или ITEM_NONE
		size_t mIndexActive;
		// индекс со свойством приема или ITEM_NONE
		size_t mIndexAccept;
		// индекс со свойством отказа или ITEM_NONE
		size_t mIndexRefuse;


		// имеем ли мы фокус ввода
		bool mIsFocus;

		// структура данных об айтеме
		VectorItemInfo mItemsInfo;

		WidgetPtr mItemDrag;
		IntPoint mPointDragOffset;

		//bool mAlignVert;

		std::string mDragLayer;

	};

} // namespace MyGUI

#endif // __MYGUI_ITEM_BOX_H__
