   mCreators->Add(gcnew System::String("Button"), gcnew HandleCreator(Button::WidgetCreator));
   mCreators->Add(gcnew System::String("Canvas"), gcnew HandleCreator(Canvas::WidgetCreator));
   mCreators->Add(gcnew System::String("ComboBox"), gcnew HandleCreator(ComboBox::WidgetCreator));
   mCreators->Add(gcnew System::String("DDContainer"), gcnew HandleCreator(DDContainer::WidgetCreator));
   mCreators->Add(gcnew System::String("Edit"), gcnew HandleCreator(EditBox::WidgetCreator));
   mCreators->Add(gcnew System::String("HScroll"), gcnew HandleCreator(HScrollBar::WidgetCreator));
   mCreators->Add(gcnew System::String("ItemBox"), gcnew HandleCreator(ItemBox::WidgetCreator));
   mCreators->Add(gcnew System::String("List"), gcnew HandleCreator(ListBox::WidgetCreator));
   mCreators->Add(gcnew System::String("MenuBar"), gcnew HandleCreator(MenuBar::WidgetCreator));
   mCreators->Add(gcnew System::String("MenuCtrl"), gcnew HandleCreator(MenuCtrl::WidgetCreator));
   mCreators->Add(gcnew System::String("Message"), gcnew HandleCreator(MessageBox::WidgetCreator));
   mCreators->Add(gcnew System::String("MultiList"), gcnew HandleCreator(MultiListBox::WidgetCreator));
   mCreators->Add(gcnew System::String("PopupMenu"), gcnew HandleCreator(PopupMenu::WidgetCreator));
   mCreators->Add(gcnew System::String("Progress"), gcnew HandleCreator(ProgressBar::WidgetCreator));
   mCreators->Add(gcnew System::String("RenderBox"), gcnew HandleCreator(RenderBox::WidgetCreator));
   mCreators->Add(gcnew System::String("ScrollView"), gcnew HandleCreator(ScrollView::WidgetCreator));
   mCreators->Add(gcnew System::String("StaticImage"), gcnew HandleCreator(StaticImage::WidgetCreator));
   mCreators->Add(gcnew System::String("StaticText"), gcnew HandleCreator(StaticText::WidgetCreator));
   mCreators->Add(gcnew System::String("Tab"), gcnew HandleCreator(TabBar::WidgetCreator));
   mCreators->Add(gcnew System::String("VScroll"), gcnew HandleCreator(VScrollBar::WidgetCreator));
   mCreators->Add(gcnew System::String("Widget"), gcnew HandleCreator(Widget::WidgetCreator));
   mCreators->Add(gcnew System::String("Window"), gcnew HandleCreator(Window::WidgetCreator));
   mCreators->Add(gcnew System::String("MenuItem"), gcnew HandleCreator(MenuItem::WidgetCreator));
   mCreators->Add(gcnew System::String("TabItem"), gcnew HandleCreator(TabItem::WidgetCreator));
