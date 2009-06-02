/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
/*
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
#include "MyGUI_Precompiled.h"
#include "MyGUI_TileRect.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_LanguageManager.h"
#include "MyGUI_LayerNode.h"

namespace MyGUI
{

	struct TileRectStateData : public StateInfo
	{
		FloatRect rect;
	};

	const size_t TILERECT_COUNT_VERTEX = 16 * VertexQuad::VertexCount;

	TileRect::TileRect(const SubWidgetInfo &_info, ICroppedRectangle * _parent) :
		ISubWidgetRect(_info.coord, _info.align, _parent),
		mEmptyView(false),
		mCurrentAlpha(0xFFFFFFFF),
		mCurrentCoord(_info.coord),
		mNode(nullptr),
		mRenderItem(nullptr),
		mTileSize(_info.coord.size()),
		mCountVertex(TILERECT_COUNT_VERTEX),
		mTileH(true),
		mTileV(true)
	{
		MapString::const_iterator iter = _info.properties.find("TileH");
		if (iter != _info.properties.end()) mTileH = utility::parseBool(iter->second);
		iter = _info.properties.find("TileV");
		if (iter != _info.properties.end()) mTileV = utility::parseBool(iter->second);

		updateTextureData();
	}

	TileRect::~TileRect()
	{
	}

	void TileRect::setVisible(bool _visible)
	{
		if (mVisible == _visible) return;
		mVisible = _visible;

		if (nullptr != mNode) mNode->outOfDate(mRenderItem);
	}

	void TileRect::setAlpha(float _alpha)
	{
		mCurrentAlpha = 0x00FFFFFF | ((uint8)(_alpha*255) << 24);
		if (nullptr != mNode) mNode->outOfDate(mRenderItem);
	}

	void TileRect::_correctView()
	{
		if (nullptr != mNode) mNode->outOfDate(mRenderItem);
	}

	void TileRect::_setAlign(const IntCoord& _oldcoord, bool _update)
	{
		_setAlign(_oldcoord.size(), _update);
	}

	void TileRect::_setAlign(const IntSize& _oldsize, bool _update)
	{

		// необходимо разобраться
		bool need_update = true;//_update;

		// первоначальное выравнивание
		if (mAlign.isHStretch())
		{
			// растягиваем
			mCoord.width = mCoord.width + (mCroppedParent->getWidth() - _oldsize.width);
			need_update = true;
			mIsMargin = true; // при изменении размеров все пересчитывать
		}
		else if (mAlign.isRight())
		{
			// двигаем по правому краю
			mCoord.left = mCoord.left + (mCroppedParent->getWidth() - _oldsize.width);
			need_update = true;
		}
		else if (mAlign.isHCenter())
		{
			// выравнивание по горизонтали без растяжения
			mCoord.left = (mCroppedParent->getWidth() - mCoord.width) / 2;
			need_update = true;
		}

		if (mAlign.isVStretch())
		{
			// растягиваем
			mCoord.height = mCoord.height + (mCroppedParent->getHeight() - _oldsize.height);
			need_update = true;
			mIsMargin = true; // при изменении размеров все пересчитывать
		}
		else if (mAlign.isBottom())
		{
			// двигаем по нижнему краю
			mCoord.top = mCoord.top + (mCroppedParent->getHeight() - _oldsize.height);
			need_update = true;
		}
		else if (mAlign.isVCenter())
		{
			// выравнивание по вертикали без растяжения
			mCoord.top = (mCroppedParent->getHeight() - mCoord.height) / 2;
			need_update = true;
		}

		if (need_update)
		{
			mCurrentCoord = mCoord;
			if (!mTileH) mTileSize.width = mCoord.width;
			if (!mTileV) mTileSize.height = mCoord.height;
			_updateView();
		}

	}

	void TileRect::_updateView()
	{
		bool margin = _checkMargin();

		mEmptyView = ((0 >= _getViewWidth()) || (0 >= _getViewHeight()));

		mCurrentCoord.left = mCoord.left + mMargin.left;
		mCurrentCoord.top = mCoord.top + mMargin.top;
		mCurrentCoord.width = _getViewWidth();
		mCurrentCoord.height = _getViewHeight();

		// подсчитываем необходимое колличество тайлов
		if (false == mEmptyView)
		{
			size_t count_x = mCoord.width / mTileSize.width;
			if ((mCoord.width % mTileSize.width) > 0) count_x ++;
			size_t count = mCoord.height / mTileSize.height;
			if ((mCoord.height % mTileSize.height) > 0) count ++;
			count = count * count_x * VertexQuad::VertexCount;

			// нужно больше вершин
			if (count > mCountVertex)
			{
				mCountVertex = count + TILERECT_COUNT_VERTEX;
				if (nullptr != mRenderItem) mRenderItem->reallockDrawItem(this, mCountVertex);
			}
		}

		// вьюпорт стал битым
		if (margin)
		{
			// проверка на полный выход за границу
			if (_checkOutside())
			{
				// запоминаем текущее состояние
				mIsMargin = margin;

				// обновить перед выходом
				if (nullptr != mNode) mNode->outOfDate(mRenderItem);
				return;
			}
		}

		// запоминаем текущее состояние
		mIsMargin = margin;

		if (nullptr != mNode) mNode->outOfDate(mRenderItem);
	}

	void TileRect::setStateData(StateInfo * _data)
	{
		TileRectStateData * data = (TileRectStateData*)_data;
		_setUVSet(data->rect);
	}

	void TileRect::_setUVSet(const FloatRect& _rect)
	{
		mCurrentTexture = _rect;
		updateTextureData();
		if (nullptr != mNode) mNode->outOfDate(mRenderItem);
	}

	void TileRect::doRender()
	{
		if (!mVisible || mEmptyView) return;

		VertexQuad* quad = (VertexQuad*)mRenderItem->getCurrentVertextBuffer();

		const RenderTargetInfo& info = mRenderItem->getRenderTarget()->getInfo();
		// unused
		//bool _update = mRenderItem->getCurrentUpdate();

		//if (_update)
		{
			//updateTextureData();
			// размер одного тайла
			mRealTileWidth = info.pixScaleX * (float)(mTileSize.width) * 2;
			mRealTileHeight = info.pixScaleY * (float)(mTileSize.height) * 2;

			mTextureHeightOne = (mCurrentTexture.bottom - mCurrentTexture.top) / mRealTileHeight;
			mTextureWidthOne = (mCurrentTexture.right - mCurrentTexture.left) / mRealTileWidth;
		}

		float vertex_z = info.maximumDepth;

		// абсолютный размер окна
		float window_left = ((info.pixScaleX * (float)(mCoord.left + mCroppedParent->getAbsoluteLeft()) + info.hOffset) * 2) - 1;
		float window_top = -(((info.pixScaleY * (float)(mCoord.top + mCroppedParent->getAbsoluteTop()) + info.vOffset) * 2) - 1);

		// размер вьюпорта
		float real_left = ((info.pixScaleX * (float)(mCurrentCoord.left + mCroppedParent->getAbsoluteLeft() - info.leftOffset) + info.hOffset) * 2) - 1;
		float real_right = real_left + (info.pixScaleX * (float)mCurrentCoord.width * 2);
		float real_top = -(((info.pixScaleY * (float)(mCurrentCoord.top + mCroppedParent->getAbsoluteTop() - info.topOffset) + info.vOffset) * 2) - 1);
		float real_bottom = real_top - (info.pixScaleY * (float)mCurrentCoord.height * 2);

		size_t count = 0;

		float left = window_left;
		float right = window_left;
		float top = window_top;
		float bottom = window_top;

		for (int y=0; y<mCoord.height; y+=mTileSize.height)
		{
			top = bottom;
			bottom -= mRealTileHeight;
			right = window_left;

			float vertex_top = top;
			float vertex_bottom = bottom;
			bool texture_crop_height  = false;

			if (vertex_top > real_top)
			{
				// проверка на полный выход
				if (vertex_bottom > real_top)
				{
					continue;
				}
				// обрезаем
				vertex_top = real_top;
				texture_crop_height = true;
			}
			if (vertex_bottom < real_bottom)
			{
				// вообще вниз ушли
				if (vertex_top < real_bottom)
				{
					continue;
				}
				// обрезаем
				vertex_bottom = real_bottom;
				texture_crop_height = true;
			}

			for (int x=0; x<mCoord.width; x+=mTileSize.width)
			{
				left = right;
				right += mRealTileWidth;

				float vertex_left = left;
				float vertex_right = right;
				bool texture_crop_width  = false;


				if (vertex_left < real_left)
				{
					// проверка на полный выход
					if (vertex_right < real_left)
					{
						continue;
					}
					// обрезаем
					vertex_left = real_left;
					texture_crop_width = true;
				}

				if (vertex_right > real_right)
				{
					// вообще строку до конца не нуна
					if (vertex_left > real_right)
					{
						continue;
					}
					// обрезаем
					vertex_right = real_right;
					texture_crop_width = true;
				}

				// текущие текстурные координаты
				float texture_left = mCurrentTexture.left;
				float texture_right = mCurrentTexture.right;
				float texture_top = mCurrentTexture.top;
				float texture_bottom = mCurrentTexture.bottom;

				// смещение текстуры по вертикили
				if (texture_crop_height)
				{
					// прибавляем размер смещения в текстурных координатах
					texture_top += (top - vertex_top) * mTextureHeightOne;
					// отнимаем размер смещения в текстурных координатах
					texture_bottom -= (vertex_bottom - bottom) * mTextureHeightOne;
				}

				// смещение текстуры по горизонтали
				if (texture_crop_width)
				{
					// прибавляем размер смещения в текстурных координатах
					texture_left += (vertex_left - left) * mTextureWidthOne;
					// отнимаем размер смещения в текстурных координатах
					texture_right -= (right - vertex_right) * mTextureWidthOne;
				}

				quad[count].set(
					vertex_left,
					vertex_top,
					vertex_right,
					vertex_bottom,
					vertex_z,
					texture_left,
					texture_top,
					texture_right,
					texture_bottom,
					mCurrentAlpha
					);

				count ++;
			}
		}

		mRenderItem->setLastVertexCount(VertexQuad::VertexCount * count);
	}

	void TileRect::createDrawItem(const std::string& _texture, ILayerNode * _node)
	{
		MYGUI_ASSERT(!mRenderItem, "mRenderItem must be nullptr");

		mNode = _node;
		mRenderItem = mNode->addToRenderItem(_texture, this);
		mRenderItem->addDrawItem(this, mCountVertex);
	}

	void TileRect::destroyDrawItem()
	{
		MYGUI_ASSERT(mRenderItem, "mRenderItem must be not nullptr");

		mNode = nullptr;
		mRenderItem->removeDrawItem(this);
		mRenderItem = nullptr;
	}

	void TileRect::updateTextureData()
	{
		// размер одного тайла
		/*mRealTileWidth = info.pixScaleX * (float)(mTileSize.width) * 2;
		mRealTileHeight = info.pixScaleY * (float)(mTileSize.height) * 2;

		mTextureHeightOne = (mCurrentTexture.bottom - mCurrentTexture.top) / mRealTileHeight;
		mTextureWidthOne = (mCurrentTexture.right - mCurrentTexture.left) / mRealTileWidth;*/
	}

	StateInfo * TileRect::createStateData(xml::ElementPtr _node, xml::ElementPtr _root, Version _version)
	{
		std::string texture = _root->findAttribute("texture");

		// поддержка замены тегов в скинах
		if (_version >= Version(1, 1))
		{
			texture = LanguageManager::getInstance().replaceTags(texture);
		}

		const IntSize & size = SkinManager::getInstance().getTextureSize(texture);
		TileRectStateData * data = new TileRectStateData();
		const FloatRect & source = FloatRect::parse(_node->findAttribute("offset"));
		data->rect = SkinManager::getInstance().convertTextureCoord(source, size);
		return data;
	}

} // namespace MyGUI
