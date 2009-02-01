﻿/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MMyGUI_Widget.h"


namespace MMyGUI
{

	public ref class DDContainer : public Widget
	{

		//--------------------------------------------------------------------
		// объявление типов и конструкторов
		MMYGUI_DECLARE_DERIVED( DDContainer, Widget );

		//InsertPoint

   	public:
		delegate void HandleInvalideContainer( Convert<MyGUI::DDContainer *>::Type _sender );
		event HandleInvalideContainer^ InvalideContainer
		{
			void add(HandleInvalideContainer^ _value)
			{
				mDelegateInvalideContainer += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventInvalideContainer =
					static_cast< MyGUI::delegates::IDelegate1< MyGUI::DDContainer * > *>(
						new Delegate1< HandleInvalideContainer^, MyGUI::DDContainer * >(mDelegateInvalideContainer) );
			}
			void remove(HandleInvalideContainer^ _value)
			{
				mDelegateInvalideContainer -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateInvalideContainer == nullptr)
					static_cast<ThisType*>(mNative)->eventInvalideContainer = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventInvalideContainer =
						static_cast< MyGUI::delegates::IDelegate1< MyGUI::DDContainer * > *>(
							new Delegate1< HandleInvalideContainer^, MyGUI::DDContainer * >(mDelegateInvalideContainer) );
			}
		}
	private:
		HandleInvalideContainer^ mDelegateInvalideContainer;




   	public:
		delegate void HandleUpdateDropState( Convert<MyGUI::DDContainer *>::Type _sender, Convert<std::vector<  MyGUI::DropWidgetInfo  > &>::Type _items, Convert<const MyGUI::DropWidgetState &>::Type _state );
		event HandleUpdateDropState^ UpdateDropState
		{
			void add(HandleUpdateDropState^ _value)
			{
				mDelegateUpdateDropState += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventUpdateDropState =
					static_cast< MyGUI::delegates::IDelegate3< MyGUI::DDContainer *, std::vector<  MyGUI::DropWidgetInfo  > &, const MyGUI::DropWidgetState & > *>(
						new Delegate3< HandleUpdateDropState^, MyGUI::DDContainer *, std::vector<  MyGUI::DropWidgetInfo  > &, const MyGUI::DropWidgetState & >(mDelegateUpdateDropState) );
			}
			void remove(HandleUpdateDropState^ _value)
			{
				mDelegateUpdateDropState -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateUpdateDropState == nullptr)
					static_cast<ThisType*>(mNative)->eventUpdateDropState = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventUpdateDropState =
						static_cast< MyGUI::delegates::IDelegate3< MyGUI::DDContainer *, std::vector<  MyGUI::DropWidgetInfo  > &, const MyGUI::DropWidgetState & > *>(
							new Delegate3< HandleUpdateDropState^, MyGUI::DDContainer *, std::vector<  MyGUI::DropWidgetInfo  > &, const MyGUI::DropWidgetState & >(mDelegateUpdateDropState) );
			}
		}
	private:
		HandleUpdateDropState^ mDelegateUpdateDropState;




   	public:
		delegate void HandleDropWidgetInfo( Convert<MyGUI::DDContainer *>::Type _sender, Convert<std::vector<  MyGUI::DropWidgetInfo  > &>::Type _items );
		event HandleDropWidgetInfo^ DropWidgetInfo
		{
			void add(HandleDropWidgetInfo^ _value)
			{
				mDelegateDropWidgetInfo += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->requestDropWidgetInfo =
					static_cast< MyGUI::delegates::IDelegate2< MyGUI::DDContainer *, std::vector<  MyGUI::DropWidgetInfo  > & > *>(
						new Delegate2< HandleDropWidgetInfo^, MyGUI::DDContainer *, std::vector<  MyGUI::DropWidgetInfo  > & >(mDelegateDropWidgetInfo) );
			}
			void remove(HandleDropWidgetInfo^ _value)
			{
				mDelegateDropWidgetInfo -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateDropWidgetInfo == nullptr)
					static_cast<ThisType*>(mNative)->requestDropWidgetInfo = nullptr;
				else
					static_cast<ThisType*>(mNative)->requestDropWidgetInfo =
						static_cast< MyGUI::delegates::IDelegate2< MyGUI::DDContainer *, std::vector<  MyGUI::DropWidgetInfo  > & > *>(
							new Delegate2< HandleDropWidgetInfo^, MyGUI::DDContainer *, std::vector<  MyGUI::DropWidgetInfo  > & >(mDelegateDropWidgetInfo) );
			}
		}
	private:
		HandleDropWidgetInfo^ mDelegateDropWidgetInfo;




   	public:
		delegate void HandleDropState( Convert<MyGUI::DDContainer *>::Type _sender, Convert<MyGUI::DropItemState>::Type _state );
		event HandleDropState^ DropState
		{
			void add(HandleDropState^ _value)
			{
				mDelegateDropState += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventDropState =
					static_cast< MyGUI::delegates::IDelegate2< MyGUI::DDContainer *, MyGUI::DropItemState > *>(
						new Delegate2< HandleDropState^, MyGUI::DDContainer *, MyGUI::DropItemState >(mDelegateDropState) );
			}
			void remove(HandleDropState^ _value)
			{
				mDelegateDropState -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateDropState == nullptr)
					static_cast<ThisType*>(mNative)->eventDropState = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventDropState =
						static_cast< MyGUI::delegates::IDelegate2< MyGUI::DDContainer *, MyGUI::DropItemState > *>(
							new Delegate2< HandleDropState^, MyGUI::DDContainer *, MyGUI::DropItemState >(mDelegateDropState) );
			}
		}
	private:
		HandleDropState^ mDelegateDropState;










   	public:
		property Convert<bool>::Type NeedDragDrop
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getNeedDragDrop() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setNeedDragDrop( Convert<bool>::From(_value) );
			}
		}



	};

} // namespace MMyGUI
