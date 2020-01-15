#include"ZTexture.h"




ZTexture::ZTexture() {

	// デバイスの取得
	m_p_device =
		Graphics::GetInstance()->GetLpDirect3DDevice9();
}


void ZTexture::Create(const char* effect_file_name) {


	//// リソースにあるz値プロットシェーダープログラムを読み込む
	//if(FAILED(D3DXCreateEffectFromResource(
	//m_p_device,
	//	NULL,
	//
	//
	//)))

	// 深度バッファサーフェイスを取得
	// Zテストを有効にするため
	// これをしないと前後関係がめちゃくちゃになる
	IDirect3DSurface9* p_tex_suf;
	m_p_device->
		GetDepthStencilSurface(&p_tex_suf);

	// サーフェイスのサイズを取得
	D3DSURFACE_DESC desc;

	m_p_tex_suf->GetDesc(&desc);

	UINT width = desc.Width;
	UINT height = desc.Height;

	// 解放
	m_p_tex_suf->Release();

	// zテクスチャ作成
	if (FAILED(
		D3DXCreateTexture(
		m_p_device,
		width,
		height,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8B8G8R8,
		D3DPOOL_DEFAULT,
		&m_p_tex
	))
		){
		return;
	}

	// zレンダリング用深度バッファ
	IDirect3DTexture9* p_z_buffer(m_p_tex);

	// Z値テクスチャの横縦サイズを保持
	UINT z_tex_width;
	UINT z_tex_height;

	//z値テクスチャサイズを割り出す
	p_z_buffer->GetSurfaceLevel(0, &m_p_tex_suf);
	GetSurfaceWH(m_p_tex_suf, z_tex_width, z_tex_height);

	m_p_device->GetDepthStencilSurface(&m_p_tex_suf);
	m_p_tex_suf->GetDesc(&desc);
	m_p_tex_suf->Release();

	// 深度バッファはテクスチャとして作成できない、
	// Zテクスチャと同じ大きさを持つ深度バッファを新規に作成する
	m_p_device->CreateDepthStencilSurface(
		z_tex_width,
		z_tex_height,
		D3DFMT_D16,
		D3DMULTISAMPLE_NONE,
		0,
		FALSE,
		&m_p_depth_buffer,
		NULL
	);

	// エフェクトファイル作成
	if (FAILED(D3DXCreateEffectFromFile(
		m_p_device,
		TEXT(effect_file_name),
		NULL, NULL, 0, NULL,&m_p_effect, NULL))) {
		return;
	}

}


void ZTexture::Begin() {

	m_p_device
		->SetDepthStencilSurface(m_p_depth_buffer);
}

void ZTexture::BeginPass() {

	m_p_effect->BeginPass(0);	// Z値計算は1パス
}

// サーフェイスの幅高取得関数
bool ZTexture::GetSurfaceWH(IDirect3DSurface9* pSurf, UINT& uiWidth, UINT& uiHeight)
{
	if (!pSurf) return false;
	D3DSURFACE_DESC SufDesc;
	pSurf->GetDesc(&SufDesc);
	uiWidth = SufDesc.Width;
	uiHeight = SufDesc.Height;

	return true;
}