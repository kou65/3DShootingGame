#include"Surface.h"
#include"../Graphics/Graphics.h"


Surface::Surface() {

	// デバイスを返す
	mp_device = 
		Graphics::GetInstance()->GetDevice();

	mp_surface = nullptr;
}


void Surface::CraeteDepthSurface(
const int &width,
const int &height,
const D3DFORMAT&format
) {

	// 深度ステンシルサーフェイス取得
	mp_device->GetDepthStencilSurface(&mp_surface);

	// サーフェイスのサイズを取得
	D3DSURFACE_DESC desc;
	mp_surface->GetDesc(&desc);

	// 深度サーフェイス作成
	mp_device->CreateDepthStencilSurface(
		width,
		height,
		desc.Format,
		desc.MultiSampleType,
		desc.MultiSampleQuality,
		TRUE,
		&mp_surface,
		NULL
	);

}


void Surface::CreateTextureSurface(
	LPDIRECT3DTEXTURE9& p_tex
) {


	// サーフェイスレベル取得
	p_tex->GetSurfaceLevel(0, &mp_surface);

}


void Surface::Release() {

	mp_surface->Release();
}