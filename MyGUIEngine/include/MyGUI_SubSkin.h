/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_SUB_SKIN_H__
#define __MYGUI_SUB_SKIN_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_Types.h"
#include "MyGUI_SubWidgetInterface.h"

namespace MyGUI
{

	class RenderItem;

	class _MyGUIExport SubSkin : public SubWidgetInterface
	{

	public:
		SubSkin(const SubWidgetInfo &_info, CroppedRectangleInterface * _parent);
		virtual ~SubSkin();

		void setAlpha(float _alpha);

		void show();
		void hide();

		void _updateView();
		void _correctView();

		void _setAlign(const IntSize& _size, bool _update);
		void _setAlign(const IntCoord& _coord, bool _update);

		
		virtual void _setUVSet(const FloatRect& _rect);
		virtual void _setStateData(void * _data);

		virtual void _createDrawItem(LayerItemKeeper * _keeper, RenderItem * _item);
		virtual void _destroyDrawItem();

		// ����� ��� ��������� ����
		virtual size_t _drawItem(Vertex * _vertex, bool _update);

		// ����� ��� ��������� ������ �� �������� xml
		static void * createStateData(xml::xmlNodePtr _node, xml::xmlNodePtr _root);

	protected:

		FloatRect mRectTexture;
		bool mEmptyView;

		uint32 mCurrentAlpha;

		FloatRect mCurrentTexture;
		IntCoord mCurrentCoord;

		RenderItem * mRenderItem;

		LayerManager * mManager;
	};

} // namespace MyGUI

#endif // __MYGUI_SUB_SKIN_H__
