/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _0bc094f6_7aec_40d5_94ce_5441eb881934_
#define _0bc094f6_7aec_40d5_94ce_5441eb881934_

#include <MyGUI.h>

/*namespace tools
{

	template <typename T>
	class ItemHolder
	{
	public:
		typedef std::vector<T*> VectorItem;
		typedef MyGUI::Enumerator<VectorItem> EnumeratorItem;

		typedef MyGUI::delegates::CMultiDelegate0 EventHandle_ChangeSelection;

	public:
		ItemHolder() :
			mItemSelected(nullptr)
		{
		}

		virtual ~ItemHolder()
		{
			destroyAllChilds();
		}

		T* createChild()
		{
			return createChild(true);
		}

		void destroyChild(T* _item)
		{
			destroyChild(_item, true);
		}

		void destroyAllChilds()
		{
			destroyAllChilds(true);
		}

		void moveItem(size_t _currentPosition, size_t _newPosition)
		{
			T* item = mChilds[_currentPosition];
			mChilds.erase(mChilds.begin() + _currentPosition);

			if (_newPosition == MyGUI::ITEM_NONE)
				_newPosition = mChilds.size();

			mChilds.insert(mChilds.begin() + _newPosition, item);

			eventChangeList();
		}

		EnumeratorItem getChildsEnumerator() const
		{
			return EnumeratorItem(mChilds);
		}

		T* getItemSelected() const
		{
			return mItemSelected;
		}

		void setItemSelected(T* _value)
		{
			mItemSelected = _value;
			eventChangeSelection();
		}

		size_t getItemCount() const
		{
			return mChilds.size();
		}

		EventHandle_ChangeSelection eventChangeSelection;
		EventHandle_ChangeSelection eventChangeList;

	protected:
		T* createChild(bool _event)
		{
			T* item = new T();
			mChilds.push_back(item);

			if (_event)
				eventChangeList();

			return item;
		}

		void destroyAllChilds(bool _event)
		{
			if (getItemSelected() != nullptr)
				setItemSelected(nullptr);

			if (!mChilds.empty())
			{
				for (typename VectorItem::iterator item = mChilds.begin(); item != mChilds.end(); ++item)
					delete *item;
				mChilds.clear();

				if (_event)
					eventChangeList();
			}
		}

		void destroyChild(T* _item, bool _event)
		{
			typename VectorItem::iterator item = std::find(mChilds.begin(), mChilds.end(), _item);
			if (item != mChilds.end())
			{
				if (*item == mItemSelected)
				{
					mItemSelected = nullptr;
					eventChangeSelection();
				}

				delete *item;
				mChilds.erase(item);
			}
			else
			{
				MYGUI_EXCEPT("item not found");
			}

			if (_event)
				eventChangeList();
		}

	private:
		VectorItem mChilds;
		T* mItemSelected;
	};

}*/

#endif
