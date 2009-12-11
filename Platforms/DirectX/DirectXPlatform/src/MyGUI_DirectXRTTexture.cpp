/*!
	@file
	@author		Albert Semenov
	@date		12/2009
	@module
*/

#include "MyGUI_Precompiled.h"
#include "MyGUI_DirectXRTTexture.h"
#include "MyGUI_DirectXRenderManager.h"

#include <d3dx9.h>

namespace MyGUI
{

	DirectXRTTexture::DirectXRTTexture(IDirect3DDevice9* _device, IDirect3DTexture9* _texture) :
		mpD3DDevice(_device),
		mpTexture(_texture),
		mpRenderSurface(NULL),
		mpBackBuffer(NULL)
	{
		mpTexture->GetSurfaceLevel(0, &mpRenderSurface);

		D3DSURFACE_DESC info;
		mpTexture->GetLevelDesc(0, &info);
		int width = info.Width;
		int height = info.Height;

		mRenderTargetInfo.maximumDepth = 0.0f;
		mRenderTargetInfo.hOffset = -0.5f / float(width);
		mRenderTargetInfo.vOffset = -0.5f / float(height);
		mRenderTargetInfo.aspectCoef = float(height) / float(width);
		mRenderTargetInfo.pixScaleX = 1.0 / float(width);
		mRenderTargetInfo.pixScaleY = 1.0 / float(height);
	}

	DirectXRTTexture::~DirectXRTTexture()
	{
	}

	void DirectXRTTexture::begin()
	{
		mpD3DDevice->GetRenderTarget(0, &mpBackBuffer);

		mpD3DDevice->SetRenderTarget(0, mpRenderSurface);
		mpD3DDevice->Clear(
			0,
			NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_ARGB(0, 0, 0, 0),
			1.0f,
			0);

		mpD3DDevice->BeginScene();
	}

	void DirectXRTTexture::end()
	{
		mpD3DDevice->EndScene();

		mpD3DDevice->SetRenderTarget(0, mpBackBuffer);
	}

	void DirectXRTTexture::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		DirectXRenderManager::getInstance().doRender(_buffer, _texture, _count);
	}

} // namespace MyGUI