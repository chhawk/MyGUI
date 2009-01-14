/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
#include "precompiled.h"
#include "DemoKeeper.h"
#include "CellView.h"

namespace demo
{

	void DemoKeeper::notifyStartDrop(wraps::BaseLayout * _sender, wraps::ItemDropInfo _info, bool & _result)
	{
		if (_info.sender_index != MyGUI::ITEM_NONE) {
			ItemData * data = *((ItemBox*)_info.sender)->getItemDataAt<ItemData*>(_info.sender_index);
			_result = !data->isEmpty();
		}
	}

	void DemoKeeper::notifyRequestDrop(wraps::BaseLayout * _sender, wraps::ItemDropInfo _info, bool & _result)
	{
		// �� �� ����� ������
		if (_info.reseiver_index == MyGUI::ITEM_NONE) {
			_result = false;
			return;
		}

		// �� ���� � �� ����� �����
		if ((_info.sender == _info.reseiver) && (_info.sender_index == _info.reseiver_index)) {
			_result = false;
			return;
		}

		ItemData * sender_data = *((ItemBox*)_info.sender)->getItemDataAt<ItemData*>(_info.sender_index);
		ItemData * reseiver_data = *((ItemBox*)_info.reseiver)->getItemDataAt<ItemData*>(_info.reseiver_index);

		_result = reseiver_data->isEmpty() || reseiver_data->compare(sender_data);
	}

	void DemoKeeper::notifyEndDrop(wraps::BaseLayout * _sender, wraps::ItemDropInfo _info, bool _result)
	{
		if (_result) {

			ItemData * sender_data = *((ItemBox*)_info.sender)->getItemDataAt<ItemData*>(_info.sender_index);
			ItemData * reseiver_data = *((ItemBox*)_info.reseiver)->getItemDataAt<ItemData*>(_info.reseiver_index);

			reseiver_data->add(sender_data);
			sender_data->clear();

			
			((ItemBox*)_info.reseiver)->setItemData(_info.reseiver_index, reseiver_data);
			((ItemBox*)_info.sender)->setItemData(_info.sender_index, sender_data);
		}

	}

	void DemoKeeper::notifyNotifyItem(wraps::BaseLayout * _sender, const MyGUI::NotifyItemData & _info)
	{
		/*if (_info.index != MyGUI::ITEM_NONE) {
			if (_info.notify == MyGUI::NotifyItem::NOTIFY_MOUSE_RELEASED) {
				MyGUI::ItemBoxPtr sender = ((BaseItemBox<CellView>*)_sender)->getItemBox();
				//sender->
				sender->setPosition(sender->getCoord());
			}
		}*/
	}

	void DemoKeeper::notifyDropState(wraps::BaseLayout * _sender, MyGUI::DropItemState _state)
	{
		/*if (_state == MyGUI::DropItemState::Refuse) MyGUI::PointerManager::getInstance().setPointer("drop_refuse", _sender->mainWidget());
		else if (_state == MyGUI::DropItemState::Accept) MyGUI::PointerManager::getInstance().setPointer("drop_accept", _sender->mainWidget());
		else if (_state == MyGUI::DropItemState::Miss) MyGUI::PointerManager::getInstance().setPointer("drop", _sender->mainWidget());
		else if (_state == MyGUI::DropItemState::Start) MyGUI::PointerManager::getInstance().setPointer("drop", _sender->mainWidget());
		else if (_state == MyGUI::DropItemState::End) MyGUI::PointerManager::getInstance().setDefaultPointer();*/
	}

	void DemoKeeper::createScene()
	{
		// ������������ ��� ������ �������
		demo::ResourceItemInfo::registryType();

		base::BaseManager::getInstance().addResourceLocation("../../Media/Demos/Demo_ItemBox");
		base::BaseManager::getInstance().addResourceLocation("../../Media/Icons");
        base::BaseManager::getInstance().addResourceLocation("../../Media/Common/Wallpapers");
        base::BaseManager::getInstance().setWallpaper("wallpaper0.jpg");
		MyGUI::Gui::getInstance().load("Resources.xml");
		MyGUI::Gui::getInstance().load("ItemBox_skin.xml");

		MyGUI::Gui * gui = MyGUI::Gui::getInstancePtr();
		int width = (int)gui->getViewWidth();
		int height = (int)gui->getViewHeight();


		mToolTip = new ToolTip();
		mToolTip->hide();

		mItemBoxV = new ItemBoxWindow("ItemBoxV.layout");
		mItemBoxV->getItemBox()->addItem(new ItemData());
		mItemBoxV->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item1", 5));
		mItemBoxV->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item2", 5));
		mItemBoxV->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item3", 5));
		mItemBoxV->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item4", 5));
		mItemBoxV->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item5", 5));
		mItemBoxV->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item6", 5));
		mItemBoxV->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item7", 5));
		mItemBoxV->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item8", 5));
		mItemBoxV->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item9", 5));

		mItemBoxV->getItemBox()->eventStartDrop = MyGUI::newDelegate(this, &DemoKeeper::notifyStartDrop);
		mItemBoxV->getItemBox()->eventRequestDrop = MyGUI::newDelegate(this, &DemoKeeper::notifyRequestDrop);
		mItemBoxV->getItemBox()->eventEndDrop = MyGUI::newDelegate(this, &DemoKeeper::notifyEndDrop);
		mItemBoxV->getItemBox()->eventDropState = newDelegate(this, &DemoKeeper::notifyDropState);
		mItemBoxV->getItemBox()->eventNotifyItem = newDelegate(this, &DemoKeeper::notifyNotifyItem);
		mItemBoxV->getItemBox()->eventToolTip = newDelegate(this, &DemoKeeper::notifyToolTip);

		mItemBoxH = new ItemBoxWindow("ItemBoxH.layout");
		mItemBoxH->getItemBox()->addItem(new ItemData());
		mItemBoxH->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item1", 5));
		mItemBoxH->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item2", 5));
		mItemBoxH->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item3", 5));
		mItemBoxH->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item4", 5));
		mItemBoxH->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item5", 5));
		mItemBoxH->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item6", 5));
		mItemBoxH->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item7", 5));
		mItemBoxH->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item8", 5));
		mItemBoxH->getItemBox()->addItem(new ItemData("info_Crystal_Clear_Item9", 5));

		mItemBoxH->getItemBox()->eventStartDrop = MyGUI::newDelegate(this, &DemoKeeper::notifyStartDrop);
		mItemBoxH->getItemBox()->eventRequestDrop = MyGUI::newDelegate(this, &DemoKeeper::notifyRequestDrop);
		mItemBoxH->getItemBox()->eventEndDrop = MyGUI::newDelegate(this, &DemoKeeper::notifyEndDrop);
		mItemBoxH->getItemBox()->eventDropState = newDelegate(this, &DemoKeeper::notifyDropState);
		mItemBoxH->getItemBox()->eventNotifyItem = newDelegate(this, &DemoKeeper::notifyNotifyItem);
		mItemBoxH->getItemBox()->eventToolTip = newDelegate(this, &DemoKeeper::notifyToolTip);


	}

	void DemoKeeper::destroyScene()
	{
		// ��� ������ �������
		demo::ResourceItemInfo::unregistryType();

		delete mItemBoxH;
		mItemBoxH = 0;

		delete mItemBoxV;
		mItemBoxV = 0;

		delete mToolTip;
		mToolTip = 0;
	}

	void DemoKeeper::notifyToolTip(wraps::BaseLayout * _sender, const MyGUI::ToolTipInfo & _info, ItemData * _data)
	{
		if (_info.type == MyGUI::ToolTipInfo::Show) {
			mToolTip->show(_data, _info.point);
		}
		else if (_info.type == MyGUI::ToolTipInfo::Hide) {
			mToolTip->hide();
		}
	}

} // namespace demo
