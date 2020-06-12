#include"ZTexture.h"




ZTexture::ZTexture(const VertexDecl::Type &type) {

	// デバイスの取得
	mp_device = 
		Graphics::GetInstance()->GetDevice();

	// 各パラメータ初期化
	D3DXMatrixIdentity(&m_mat_world);
	D3DXMatrixIdentity(&m_mat_view);
	D3DXMatrixIdentity(&m_mat_proj);

	m_h_world = NULL;
	m_h_view_mat = NULL;
	m_h_proj_mat = NULL;

	m_start_pass_num = 0;

	// フォーマット初期化
	m_z_tex_format = D3DFMT_A8B8G8R8;

	// エフェクトファイル作成
	CreateEffectFile(
		"Lib/Shader/EffectFile/ZTexture.fx",
		"ZValuePlotTec",
		type
	);
}


void ZTexture::Init() {

	// ハンドルの初期化
	InitHandle();

	// テクスチャ作成
	CreateTexture(
		m_z_tex_width,
		m_z_tex_height,
		m_z_tex_format
	);

	// Zテクスチャ
	InitZTexture();
}


void ZTexture::InitHandle() {

	StandardTSShader::InitTSHandle(
		"matWorld",
		"matView",
		"matProj"
	);
}


bool ZTexture::CreateTexture(
	UINT z_tex_width,
	UINT z_tex_height,
	D3DFORMAT z_tex_format) {

	// zテクスチャ作成(別でも良い)
	// 与えたサイズではない2のべき乗のテクスチャで返すことがある
	if (FAILED(
		D3DXCreateTexture(
			mp_device,
			z_tex_width,
			z_tex_height,
			1, // ミップマップではない
			D3DUSAGE_RENDERTARGET,
			z_tex_format,
			D3DPOOL_DEFAULT,
			// 作成した空のテクスチャが返る
			&mp_tex
		))
		) {
		return false;
	}

	// 深度バッファサーフェイスレベル取得
	mp_tex->GetSurfaceLevel(0, &mp_tex_suf);
}


bool ZTexture::InitZTexture() {


	// 深度バッファサーフェイス(面)を取得
	// Zテストを有効にするため
	// これをしないと前後関係がめちゃくちゃになる

	IDirect3DSurface9 * p_sur;

	// 深度ステンシルサーフェイス取得
	mp_device->GetDepthStencilSurface(&p_sur);

	// サーフェイスのサイズを取得
	D3DSURFACE_DESC desc;
	p_sur->GetDesc(&desc);
	// 解放(内部カウンタを減らす)
	p_sur->Release();

	// ステンシルバッファ作成(深度バッファ)
	// 深度バッファはテクスチャとして作成できない、
	// Zテクスチャと同じ大きさを持つ深度バッファを新規に作成する
	HRESULT hr = mp_device->CreateDepthStencilSurface(
		m_z_tex_width,
		m_z_tex_height,
		desc.Format,
		desc.MultiSampleType,
		desc.MultiSampleQuality,
		FALSE,
		&mp_depth_buffer,// 深度バッファ
		NULL
	);

	if (hr != S_OK) {
		return false;
	}

	return true;
}


void ZTexture::Update() {
	
	// 登録されているパラメータ情報をエフェクトにセット
	StandardTSShader::Update();

	m_decl.Set();

	mp_effect->SetMatrix(m_h_world, &m_mat_world);
	mp_effect->SetMatrix(m_h_view_mat, &m_mat_view);
	mp_effect->SetMatrix(m_h_proj_mat, &m_mat_proj);
}


void ZTexture::SetTextureSize(
	UINT z_tex_width,
	UINT z_tex_height
) {

	m_z_tex_width = z_tex_width;
	m_z_tex_height = z_tex_height;
}



void ZTexture::Begin(
	UINT &total_pass_num,
	const DWORD &device_state_num) {

	if (mp_effect == nullptr) {
		return;
	}

	if (mp_depth_buffer == nullptr) {
		return;
	}

	if (mp_tex == nullptr) {
		return;
	}

	// レンダーターゲットを切り替える

	// 元のバックバッファをレンダリングターゲットを変更する前に取得
	// 現在デバイスが持っているバッファを一時保存
	mp_device->
		GetRenderTarget(0, &mp_device_buffer);

	// 深度バッファ用サーフェイスを取得
	mp_device->
		GetDepthStencilSurface(&mp_device_depth);
	
	// デバイスにzテクスチャをレンダーターゲットに設定する
	mp_device->
		SetRenderTarget(0, mp_tex_suf);
	mp_device->
		SetDepthStencilSurface(mp_depth_buffer);

	// テクスチャサーフェイスのクリア
	mp_device->Clear(0, NULL, D3DCLEAR_TARGET | 
		D3DCLEAR_ZBUFFER, 
		// 背景色も変更
		D3DCOLOR_ARGB(255, 255, 255, 255),
		1.0f,
		0
	);

	mp_effect->SetTechnique(m_h_technique);

	HRESULT hr = mp_effect->Begin(
		&total_pass_num, 
		device_state_num
	);

	if (hr != S_OK) {
		hr = S_OK;
		return;
	}
}


void ZTexture::End() {

	// 後始末
	// ちゃんと後始末しないとカウンタが狂って解放されない

	HRESULT hr = mp_effect->End();

	// 追加(テクスチャサーフェイス受け取り)
	//mp_device->
	//	GetRenderTarget(0, &mp_tex_suf);
	//mp_device->
	//	GetDepthStencilSurface(&mp_depth_buffer);

	// 深度バッファを元に戻す
	// デバイスに元のサーフェイスを戻す
	mp_device->SetRenderTarget(0, mp_device_buffer);
	mp_device->SetDepthStencilSurface(mp_device_depth);

	// 初期化
	mp_device_buffer = NULL;
	mp_device_depth = NULL;
	
	// GetSurfaceLevelで内部カウンタが+1されているのでReleaseして内部カウンタを減らす
	mp_device->SetVertexShader(NULL);
	mp_device->SetPixelShader(NULL);


	if (hr != S_OK) {
		hr = S_OK;
		return;
	}
}


bool ZTexture::GetSurfaceWH(IDirect3DSurface9* pSurf, UINT& uiWidth, UINT& uiHeight)
{

	if (!pSurf) { 
		return false;
	}

	D3DSURFACE_DESC SufDesc;
	pSurf->GetDesc(&SufDesc);
	uiWidth = SufDesc.Width;
	uiHeight = SufDesc.Height;

	return true;
}


IDirect3DTexture9 *ZTexture::GetZTexture() {

	return mp_tex;
}