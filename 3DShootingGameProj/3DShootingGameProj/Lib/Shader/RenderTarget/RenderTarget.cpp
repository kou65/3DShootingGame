#include"RenderTarget.h"



RenderTarget::RenderTarget() {

	mp_tex = nullptr;
	mp_tex_surface = nullptr;
	mp_depth = nullptr;
	m_width_size = 1024;
	m_height_size = 1024;

	// デバイス取得
	mp_device =
	Graphics::GetInstance()->GetDevice();
}


RenderTarget::~RenderTarget() {

	mp_depth->Release();
	mp_tex->Release();
	mp_tex_surface->Release();
}


bool RenderTarget::CreateSurface(
	RenderTargetData&data
) {

	if (mp_device == nullptr) {
		return false;
	}

	m_width_size = data.width;
	m_height_size = data.height;

	// 作成
	CreateTexture();
	CreateDepth();
	
	// ビューポート作成
	CreateViewPort(
		data.view_port_pos,
		data.view_port_size_w,
		data.view_port_size_h
	);

	return true;
}


void RenderTarget::DestorySurface() {

	mp_depth->Release();
	mp_tex_surface->Release();
	mp_tex->Release();

	mp_depth = nullptr;
	mp_tex_surface = nullptr;
	mp_tex = nullptr;
}


void RenderTarget::SetRenderTarget(const int&target) {

	// レンダーターゲットセット
	mp_device->
		SetRenderTarget(target, mp_tex_surface);
}


void RenderTarget::SetDepth() {

	mp_device->SetDepthStencilSurface(mp_depth);
}


void RenderTarget::GetRenderTarget(
	LPDIRECT3DSURFACE9&suf,
	const int&target) {

	// レンダーターゲット取得
	mp_device->GetRenderTarget(target, &suf);
}


void RenderTarget::GetDepth(LPDIRECT3DSURFACE9&suf) {
	mp_device->GetDepthStencilSurface(&suf);
}


bool RenderTarget::CreateTexture() {

	// レンダリングターゲットとしてテクスチャ領域確保
	HRESULT hr = D3DXCreateTexture(
		mp_device,
		m_width_size,
		m_height_size,
		1, // ミップマップではない
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8B8G8R8,
		D3DPOOL_DEFAULT,
		// 作成した空のテクスチャが返る
		&mp_tex
	);

	// 作成したテクスチャからサーフェイス取得
	mp_tex->GetSurfaceLevel(0, &mp_tex_surface);

	if (hr != S_OK) {
		return false;
	}

	return true;
}


bool RenderTarget::CreateDepth() {


	IDirect3DSurface9 * p_sur;

	// 現在の深度ステンシルサーフェイス取得
	mp_device->GetDepthStencilSurface(&p_sur);

	// サーフェイスのサイズを取得
	D3DSURFACE_DESC desc;
	p_sur->GetDesc(&desc);
	// 解放(内部カウンタを減らす)
	p_sur->Release();

	// ステンシルバッファ作成(深度バッファ)
	// 深度バッファはテクスチャとして作成できない、
	// Zテクスチャと同じ大きさを持つ深度バッファを新規に作成する
	HRESULT hr = mp_device
		->CreateDepthStencilSurface(
			m_width_size,
			m_height_size,
			desc.Format, // D3DFMT_D16
			desc.MultiSampleType, // D3DMULTISAMPLE_NONE
			desc.MultiSampleQuality, // 0
			FALSE,
			&mp_depth,// 深度バッファ
			NULL
		);

	if (hr != S_OK) {
		return false;
	}

	return true;
}


bool RenderTarget::CreateTexSurface(){

	// 作成したテクスチャからサーフェイス取得
	mp_tex->GetSurfaceLevel(0, &mp_tex_surface);
	return true;
}


void RenderTarget::GetTexture(LPDIRECT3DTEXTURE9&tex) {
	tex = mp_tex;
}


LPDIRECT3DTEXTURE9 RenderTarget::GetTexture() {
	return mp_tex;
}


void RenderTarget::CreateViewPort(
	Vec2 pos,
	DWORD width, 
	DWORD height
) {

	m_view_port.X = (DWORD)pos.x;
	m_view_port.Y = (DWORD)pos.y;

	m_view_port.Width = width;
	m_view_port.Height = height;

	m_view_port.MinZ = 0.f;
	m_view_port.MaxZ = 1.f;
}


void RenderTarget::SetViewPort() {

	//mp_device->SetViewport(&m_view_port);
}


bool RenderTarget::CheckSurface() {

	if (mp_depth == nullptr) {
		return false;
	}

	if (mp_device == nullptr) {
		return false;
	}

	if (mp_tex == nullptr) {
		return false;
	}

	if (mp_tex_surface == nullptr) {
		return false;
	}

	return true;
}