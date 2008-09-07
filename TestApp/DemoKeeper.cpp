/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "DemoKeeper.h"
#include "MyGUI_ResourceImageSet.h"
#include "MyGUI_ResourceManager.h"

namespace demo
{

	/*class Guid
	{
	public:
		Guid() { fast._data1 = fast._data2 = fast._data3 = fast._data4 = 0; }
		Guid( Guid const & _value ) { *this = _value; }
		explicit Guid(const std::string& _value) { *this = parse(_value); }
		
		inline bool operator == (Guid const & _comp) const
		{
			return _comp.fast._data1 == fast._data1
				&& _comp.fast._data2 == fast._data2
				&& _comp.fast._data3 == fast._data3
				&& _comp.fast._data4 == fast._data4;
		}

		inline bool operator != ( Guid const & _comp ) const
		{
			return ! (*this == _comp);
		}

		inline Guid & operator = (Guid const & _rvalue)
		{
			fast._data1 = _rvalue.fast._data1;
			fast._data2 = _rvalue.fast._data2;
			fast._data3 = _rvalue.fast._data3;
			fast._data4 = _rvalue.fast._data4;
			return *this;
		}

		inline bool empty() const
		{
			return fast._data1 == 0
				&& fast._data2 == 0
				&& fast._data3 == 0
				&& fast._data4 == 0;
		}

		inline void clear()
		{
			fast._data1 = fast._data2 = fast._data3 = fast._data4 = 0; 
		}

		std::string print() const;
		static Guid parse(const std::string& _value);
		static Guid generate();

        inline friend std::ostream& operator << ( std::ostream& _stream, const Guid &  _value )
        {
            _stream << _value.print();
            return _stream;
        }

        inline friend std::istream& operator >> ( std::istream& _stream, Guid &  _value )
        {
			std::string value;
            _stream >> value;
			if (_stream.fail()) _value.clear();
			else _value = Guid::parse(value);
            return _stream;
        }

	private:
		// ������ ��� ������� �����������
		static const char convert_hex[64];

		union {
			struct _original { 
				MyGUI::uint32 data1;
				MyGUI::uint16 data2, data3;
				MyGUI::uint8 data4[8];
			} original;
			struct _fast { 
				MyGUI::uint32 _data1, _data2, _data3, _data4;
			} fast;
		};
	};

	const char Guid::convert_hex[64] = {0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0};

	Guid Guid::parse(const std::string& _value)
	{
		Guid ret;
		size_t pos=0;
		// { ... }
		if (_value.size() == 38) pos ++;
		// ...
		else if (_value.size() != 36) {
			// error
			return ret;
		}

		for (; pos<9; ++pos) {
			ret.original.data1 <<= 4;
			ret.original.data1 += convert_hex[ ((_value[pos]) - 48) & 0x3F];
		}
		for (size_t pos=10; pos<14; ++pos) {
			ret.original.data2 <<= 4;
			ret.original.data2 += convert_hex[ ((_value[pos]) - 48) & 0x3F];
		}
		for (size_t pos=15; pos<19; ++pos) {
			ret.original.data3 <<= 4;
			ret.original.data3 += convert_hex[ ((_value[pos]) - 48) & 0x3F];
		}
		size_t num = 0;
		for (size_t pos=20; pos<24;) {
			ret.original.data4[num] = convert_hex[ ((_value[pos++]) - 48) & 0x3F] << 4;
			ret.original.data4[num++] += convert_hex[ ((_value[pos++]) - 48) & 0x3F];
		}
		for (size_t pos=25; pos<37;) {
			ret.original.data4[num] = convert_hex[ ((_value[pos++]) - 48) & 0x3F] << 4;
			ret.original.data4[num++] += convert_hex[ ((_value[pos++]) - 48) & 0x3F];
		}
	    return ret;
	}

	std::string Guid::print() const
	{
		const size_t SIZE = 39;
		char buff[SIZE];

		sprintf(buff, "{%.8X-%.4X-%.4X-%.2X%.2X-%.2X%.2X%.2X%.2X%.2X%.2X}\0", (int)(original.data1), (int)(original.data2), (int)(original.data3),
			(int)(original.data4[0]), (int)(original.data4[1]),
			(int)(original.data4[2]), (int)(original.data4[3]), (int)(original.data4[4]), (int)(original.data4[5]), (int)(original.data4[6]), (int)(original.data4[7])
			);

		return buff;
	}

	Guid Guid::generate()
	{
		Guid ret;
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		GUID guid;
		HRESULT result = CoCreateGuid(&guid);
		MYGUI_ASSERT(S_OK == result, "Error generate GUID");
		ret.original.data1 = guid.Data1;
		ret.original.data2 = guid.Data2;
		ret.original.data3 = guid.Data3;
		memcpy(ret.original.data4, guid.Data4, 8);
#else
#endif
		return ret;
	}*/


	void DemoKeeper::createScene()
	{

		using namespace MyGUI;
		const IntSize & view = Gui::getInstance().getViewSize();
		const IntSize size(150, 24);

		base::BaseManager::getInstance().addResourceLocation("../../Media/TestApp");
		base::BaseManager::getInstance().setWallpaper("wallpaper4.jpg");

		//EditPtr edit = Gui::getInstance().createWidget<Edit>("Memo", IntCoord((view.width - size.width) / 2, (view.height - size.height) / 2, size.width, size.height), Align::Default, "Main");
		//edit->setCaption("seig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshs seig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshs");

		/*EditPtr button = Gui::getInstance().createWidget<Edit>("Edit", IntCoord(100, 200, 59, 26), Align::Default, "Main");
		button->eventMouseButtonClick = newDelegate(this, &DemoKeeper::notifyMouseButtonClick);
		button->setCaption("press me =)");

		ButtonPtr button2 = Gui::getInstance().createWidget<Button>("ButtonMinusPlus", IntCoord(100, 300, 18, 18), Align::Default, "Main");
		button2->eventMouseButtonClick = newDelegate(this, &DemoKeeper::notifyMouseButtonClick);
		button2->setCaption("press me =)");*/
		//button->setStateCheck(true);
		//button->setEnabled(false);

		//Gui::getInstance().createWidget<ComboBox>("ComboBox", IntCoord(100, 100, 200, 26), Align::Default, "Main");

		ResourceImageSet::registryType();
		Gui::getInstance().load("resource.xml");

		StaticImagePtr image = Gui::getInstance().createWidget<StaticImage>("StaticImage", IntCoord(100, 100, 100, 100), Align::Default, "Main");

		ResourceImageSet * info = static_cast<ResourceImageSet*>(ResourceManager::getInstance().getResource(Guid("{0FFECB2A-0BC4-4799-83BD-F8F98F8D2541}")));
		image->setImageInfo(info->getIndexInfo("ItemBox", "normal"));

		//MyGUI::Gui::getInstance().load("test.font");

		createFont(80, 8);
		//MyGUI::FontManager::getInstance().saveFontTexture("font_tmp", "font.png");

		MyGUI::EditPtr edit = MyGUI::Gui::getInstance().createWidget<MyGUI::Edit>("Memo", MyGUI::IntCoord(100, 100, size.width, size.height), MyGUI::Align::Default, "Main");
		edit->setTextAlign(MyGUI::Align::Default);

		edit->setFontName("font_tmp");
		int height = static_cast<MyGUI::FontPtr>(MyGUI::FontManager::getInstance().getByName("font_tmp"))->getHeightPix();
		edit->setFontHeight(height);
		mInfo->change("font pix", MyGUI::utility::toString(height));

		edit->setCaption(L"01234567890 ABCDEFGHIJKLMNOPQRSTUVWXYZ \
			abcdefghijklmnopqrstuvwxyz \
			0 1 2 3 4 5 6 7 8 9 0 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z \
			b c d e f g h i j k l m n o p q r s t u v w x y z \
			�����Ũ������������������������� \
			�������������������������������\
			� � � � � � � � � � � � � � � � � � � � � � � � � � � � � � � � \
			� � � � � � � � � � � � � � � � � � � � � � � � � � � � � � � �");
		edit->setTextCursor(0);

		edit->setFontName("Comic.18");

		MyGUI::FontManager::getInstance().remove("font_tmp");

		createFont(80, 80);

		edit->setFontName("font_tmp");
		height = static_cast<MyGUI::FontPtr>(MyGUI::FontManager::getInstance().getByName("font_tmp"))->getHeightPix();
		edit->setFontHeight(height);
		mInfo->change("font pix", MyGUI::utility::toString(height));

	}

	void DemoKeeper::destroyScene()
	{
	}

	void DemoKeeper::createFont(int _res, int _height)
	{
		MyGUI::xml::xmlDocument document;
		document.createInfo();
		MyGUI::xml::xmlNodePtr root = document.createRoot("MyGUI");
		root->addAttributes("type", "Font");

		MyGUI::xml::xmlNodePtr node = root->createChild("Font");
		node->addAttributes("name", "font_tmp");
		node->addAttributes("default_height", "11");
		node->addAttributes("source", "Comic.ttf");
		node->addAttributes("size", MyGUI::utility::toString(_height));
		node->addAttributes("resolution", MyGUI::utility::toString(_res));
		node->addAttributes("antialias_colour", "false");
		node->addAttributes("space_width", "4");
		node->addAttributes("tab_width", "8");
		node->addAttributes("cursor_width", "2");
		node->addAttributes("distance", "5");
		node->addAttributes("offset_height", "0");

		node->createChild("Code")->addAttributes("range", "33 126");
		node->createChild("Code")->addAttributes("range", "1025 1105");

		MyGUI::FontManager::getInstance()._load(root, "");

	}

} // namespace demo
