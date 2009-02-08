/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>

#include "MMyGUI_Utility.h"

#include "Generate/MMyGUI_MarshalingIncludeWidget.h"

namespace MMyGUI
{

	public ref class Gui
	{
	private:
		Gui() { }

	public:
		static property Gui^ Instance
		{
			Gui^ get( )
			{
				if (mGui == nullptr)
				{
					mGui = MyGUI::Gui::getInstancePtr();
					mInputManager = MyGUI::InputManager::getInstancePtr();
					mLayerManager = MyGUI::LayerManager::getInstancePtr();
				}
				return m_instance;
			}
		}

	public:
		generic <typename T> where T : ref class
		T CreateWidget(System::String^ _skin, IntCoord _coord, Align _align, System::String^ _layer, System::String^ _name)
		{
			Widget^ child = (Widget^)(System::Activator::CreateInstance<T>());
			child->CreateWidget(nullptr, MyGUI::WidgetStyle::Overlapped, Convert<const std::string&>::From(_skin), Convert<const MyGUI::IntCoord&>::From(_coord), Convert<MyGUI::Align>::From(_align), Convert<const std::string&>::From(_layer), Convert<const std::string&>::From(_name));
			return (T)child;
		}

	public:
		generic <typename T> where T : ref class
		T CreateWidget(System::String^ _skin, IntCoord _coord, Align _align, System::String^ _layer)
		{
			Widget^ child = (Widget^)(System::Activator::CreateInstance<T>());
			child->CreateWidget(nullptr, MyGUI::WidgetStyle::Overlapped, Convert<const std::string&>::From(_skin), Convert<const MyGUI::IntCoord&>::From(_coord), Convert<MyGUI::Align>::From(_align), Convert<const std::string&>::From(_layer), "");
			return (T)child;
		}

	public:
		property bool KeyFocus
		{
			bool get() { return mInputManager->getKeyFocusWidget() != nullptr; }
		}

	public:
		property bool MouseFocus
		{
			bool get() { return mInputManager->getMouseFocusWidget() != nullptr; }
		}

	public:
		property Widget^ KeyFocusWidget
		{
			Widget^ get() { return Convert< MyGUI::Widget * >::To(mInputManager->getKeyFocusWidget()); }
			void set(Widget^ _widget) { mInputManager->setKeyFocusWidget( Convert< MyGUI::Widget * >::From(_widget) ); }
		}

	public:
		property Widget^ MouseFocusWidget
		{
			Widget^ get() { return Convert< MyGUI::Widget * >::To(mInputManager->getMouseFocusWidget()); }
		}

	public:
		void ResetKeyFocus()
		{
			mInputManager->setKeyFocusWidget(nullptr);
		}

	public:
		void LoadResource(System::String^ _source, System::String^ _group)
		{
			mGui->load( Convert< const std::string & >::From(_source), Convert< const std::string & >::From(_group) );
		}

	public:
		void AddWidgetModal(Widget^ _widget)
		{
			mInputManager->addWidgetModal( Convert< MyGUI::Widget * >::From(_widget) );
		}

	public:
		void RemoveWidgetModal(Widget^ _widget)
		{
			mInputManager->removeWidgetModal( Convert< MyGUI::Widget * >::From(_widget) );
		}

	public:
		void UpWidget(Widget^ _widget)
		{
			mLayerManager->upLayerItem( Convert< MyGUI::Widget * >::From(_widget) );
		}

	public:
		System::Collections::Generic::List<Widget^>^ LoadLayout(System::String^ _file)
		{
			return LoadLayout(_file, nullptr, "");
		}

	public:
		System::Collections::Generic::List<Widget^>^ LoadLayout(System::String^ _file, Widget^ _parent)
		{
			return LoadLayout(_file, _parent, "");
		}

	public:
		System::Collections::Generic::List<Widget^>^ LoadLayout(System::String^ _file, System::String^ _prefix)
		{
			return LoadLayout(_file, nullptr, _prefix);
		}

	public:
		void AttachWidgetToLayer(Widget^ _widget, System::String^  _layer)
		{
			mLayerManager->attachToLayerKeeper( utility::managed_to_utf8(_layer) , Convert< MyGUI::Widget * >::From(_widget) );
		}

	public:
		System::Collections::Generic::List<Widget^>^ LoadLayout(System::String^ _file, Widget^ _parent, System::String^ _prefix)
		{
			const std::string& file = utility::managed_to_utf8(_file);
			const std::string& prefix = utility::managed_to_utf8(_prefix);

			MyGUI::xml::Document doc;
			if ( ! doc.open(file, MyGUI::ResourceManager::getInstance().getResourceGroup()) )
			{
				MYGUI_EXCEPT("MMyGUI::Gui : '" << file << "', " << doc.getLastError());
			}

			MyGUI::xml::ElementPtr root = doc.getRoot();
			if ( (nullptr == root) || (root->getName() != "MyGUI") )
			{
				MYGUI_EXCEPT("MMyGUI::Gui : '" << file << "', tag 'MyGUI' not found");
			}

			if (root->findAttribute("type") != "Layout")
			{
				MYGUI_EXCEPT("MMyGUI::Gui : '" << file << "', attribute 'type' != 'Layout'");
			}

			MyGUI::Version version = MyGUI::Version::parse(root->findAttribute("version"));

			System::Collections::Generic::List<Widget^>^ widgets = gcnew System::Collections::Generic::List<Widget^>();

			MyGUI::xml::ElementEnumerator widgets_array = root->getElementEnumerator();
			while(widgets_array.next("Widget"))
			{
				ParseWidget(widgets_array.current(), version, widgets, _parent, prefix);
			}

			return widgets;
		}

   	public:
		delegate void HandleParserUserData( Widget^ _widget, System::String^ _key, System::String^ _value );
		event HandleParserUserData^ EventParserUserData
		{
			void add(HandleParserUserData^ _value)
			{
				mDelegateParserUserData += _value;
			}
			void remove(HandleParserUserData^ _value)
			{
				mDelegateParserUserData -= _value;
			}
		}
	private:
		HandleParserUserData^ mDelegateParserUserData;

	private:
		void ParseWidget(MyGUI::xml::ElementPtr _widget, MyGUI::Version _version, System::Collections::Generic::List<Widget^>^ _widgets, Widget^ _parent, const std::string& _prefix)
		{
			// ������ �������� �������
			std::string type, skin, name, layer, tmp;
			MyGUI::IntCoord coord;
			MyGUI::Align align = MyGUI::Align::Default;

			_widget->findAttribute("type", type);
			_widget->findAttribute("skin", skin);
			_widget->findAttribute("name", name);
			_widget->findAttribute("layer", layer);
			if (_widget->findAttribute("align", tmp)) align = MyGUI::Align::parse(tmp);
			if (_widget->findAttribute("position", tmp)) coord = MyGUI::IntCoord::parse(tmp);
			if (_widget->findAttribute("position_real", tmp)) coord = MyGUI::WidgetManager::getInstance().convertRelativeToInt(MyGUI::FloatCoord::parse(tmp), _parent->GetNativePtr());

			if (!name.empty()) name = _prefix + name;

			Widget^ wid = nullptr;
			if (nullptr == _parent)
			{
				wid = CreateWidget(_parent, MyGUI::WidgetStyle::Overlapped, type, skin, coord, align, layer, name);
			}
			else
			{
				MyGUI::WidgetStyle style = MyGUI::WidgetStyle::Child;
				if (_widget->findAttribute("style", tmp)) style = MyGUI::WidgetStyle::parse(tmp);
				if (style != MyGUI::WidgetStyle::Popup && !layer.empty()) layer.clear();

				wid = CreateWidget(_parent, style, type, skin, coord, align, layer, name);
			}

			// ����� ����� � ��������
			MyGUI::xml::ElementEnumerator widget_element = _widget->getElementEnumerator();
			while (widget_element.next())
			{
				std::string key, value;

				if (widget_element->getName() == "Widget")
				{
					ParseWidget(widget_element.current(), _version, _widgets, wid, _prefix);
				}
				else if (widget_element->getName() == "Property")
				{
					// ������ ��������
					if (false == widget_element->findAttribute("key", key)) continue;
					if (false == widget_element->findAttribute("value", value)) continue;
					// � ������ ��������
					MyGUI::WidgetManager::getInstance().parse(wid->GetNativePtr(), key, value);
				}
				else if (widget_element->getName() == "UserString")
				{
					// ������ ��������
					if (false == widget_element->findAttribute("key", key)) continue;
					if (false == widget_element->findAttribute("value", value)) continue;
					wid->GetNativePtr()->setUserString(key, value);

					if (mDelegateParserUserData != nullptr) mDelegateParserUserData(wid, utility::utf8_to_managed(key), utility::utf8_to_managed(value));
				}

			}
		}

		Widget^ CreateWidget(Widget^ _parent, MyGUI::WidgetStyle _style, const std::string& _type, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
		{
			return mCreators[ utility::utf8_to_managed(_type) ] ( _parent, _style, _skin, _coord, _align, _layer, _name );
		}

	private:
		static Gui^ m_instance = gcnew Gui();
		static MyGUI::Gui * mGui = nullptr;
		static MyGUI::InputManager* mInputManager = nullptr;
		static MyGUI::LayerManager* mLayerManager = nullptr;

		delegate Widget^ HandleCreator(Widget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name);
		static System::Collections::Generic::Dictionary<System::String^, HandleCreator^>^ mCreators = gcnew System::Collections::Generic::Dictionary<System::String^, HandleCreator^>();

		static Gui()
		{

			#include "Generate/MMyGUI_MarshalingRegistryWidget.h"

		}

	};

} // namespace MMyGUI
