/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/
#ifndef __ITEM_BOXV_H__
#define __ITEM_BOXV_H__

#include <MyGUI.h>
#include "BaseLayout.h"
#include "CellView.h"
#include "ItemData.h"
#include "ItemDropInfo.h"

class ItemBoxVLayout : public BaseLayout
{
public:
	ItemBoxVLayout();

	virtual void initialise();
	virtual void shutdown();

	void addItem(ItemData * _data);

private:
	void requestCreateWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::WidgetItemData & _info);
	void requestCoordWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::IntCoord & _coord, bool _drop);
	void requestUpdateWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::WidgetItemData _item, const MyGUI::ItemInfo& _data);

	void notifyStartDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool & _result);
	void notifyRequestDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool & _result);
	void notifyEndDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool _result);

	void notifyDropState(MyGUI::WidgetPtr _sender, MyGUI::DropState _state);

	void notifyToolTip(MyGUI::WidgetPtr _sender, const MyGUI::ToolTipInfo & _info);

public:
	MyGUI::delegates::CDelegate3<ItemBoxVLayout *, ItemDropInfo, bool &> eventStartDrop;
	MyGUI::delegates::CDelegate3<ItemBoxVLayout *, ItemDropInfo, bool &> eventRequestDrop;
	MyGUI::delegates::CDelegate3<ItemBoxVLayout *, ItemDropInfo, bool> eventEndDrop;

	MyGUI::delegates::CDelegate2<ItemBoxVLayout *, MyGUI::DropState> eventDropState;

	MyGUI::delegates::CDelegate3<ItemBoxVLayout *, const MyGUI::ToolTipInfo &, ItemData *> eventToolTip;

private:
	MyGUI::ItemBoxPtr mBoxItems;
	typedef std::vector<CellView*> VectorCellView;
	VectorCellView mListCellView;
};

#endif // __ITEM_BOXV_H__