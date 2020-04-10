#include"ZTexture.h"




ZTexture::ZTexture() {

	// デバイスの取得
	m_p_device = 
		Graphics::GetInstance()->GetDevice();

	// 各パラメータ初期化
	D3DXMatrixIdentity(&m_world_mat);
	D3DXMatrixIdentity(&m_view_mat);
	D3DXMatrixIdentity(&m_proj_mat);
	m_h_world_mat = NULL;
	m_h_view_mat = NULL;
	m_h_proj_mat = NULL;
	m_start_pass_num = 0;

	// フォーマット初期化
	m_z_tex_format = D3DFMT_A8B8G8R8;

	// エフェクトファイル作成
	CreateEffectFile(
		"Lib/ShaderFile/ZTexture.fx",
		"ZValuePlotTec",
		VertexDecl::Type::OBJ
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

	StandardTransformShader::InitTransformHandle(
		"matWorld",
		"matView",
		"matProj"
	);
}


bool ZTexture::InitZTexture() {


	// 深度バッファサーフェイス(面)を取得
	// Zテストを有効にするため
	// これをしないと前後関係がめちゃくちゃになる

	IDirect3DSurface9 * p_sur;

	// 深度ステンシルサーフェイス取得
	m_p_device->GetDepthStencilSurface(&p_sur);

	// サーフェイスのサイズを取得
	D3DSURFACE_DESC desc;
	p_sur->GetDesc(&desc);
	// 解放(内部カウンタを減らす)
	p_sur->Release();

	// ステンシルバッファ作成(深度バッファ)
	// 深度バッファはテクスチャとして作成できない、
	// Zテクスチャと同じ大きさを持つ深度バッファを新規に作成する
	HRESULT hr = m_p_device->CreateDepthStencilSurface(
		m_z_tex_width,
		m_z_tex_height,
		desc.Format,
		desc.MultiSampleType,
		desc.MultiSampleQuality,
		FALSE,
		&m_p_depth_buffer,
		NULL
	);

	if (hr != S_OK) {
		return false;
	}

	return true;
}


void ZTexture::Update() {

	StandardTransformShader::UpdateTransfromMatrix();
}


void ZTexture::SetTextureSize(
	UINT z_tex_width,
	UINT z_tex_height
) {

	m_z_tex_width = z_tex_width;
	m_z_tex_height = z_tex_height;
}


bool ZTexture::CreateTexture(
	UINT z_tex_width,
	UINT z_tex_height,
	D3DFORMAT z_tex_format) {

	// zテクスチャ作成(別でも良い)
	// 与えたサイズではない2のべき乗のテクスチャで返すことがある
	if (FAILED(
		D3DXCreateTexture(
			m_p_device,
			z_tex_width,
			z_tex_height,
			1,
			D3DUSAGE_RENDERTARGET,
			z_tex_format,
			D3DPOOL_DEFAULT,
			// 作成した空のテクスチャが返る
			&m_p_tex
		))
		) {
		return false;
	}

	// 深度バッファサーフェイスレベル取得
	m_p_tex->GetSurfaceLevel(0, &m_p_tex_suf);

}


void ZTexture::Begin(
	UINT &total_pass_num,
	const DWORD &device_state_num) {

	if (m_p_effect == nullptr) {
		return;
	}

	// レンダーターゲットを切り替える

	// 元のバックバッファをレンダリングターゲットを変更する前に取得
	// 現在デバイスが持っているバッファを一時保存
	m_p_device->
		GetRenderTarget(0, &m_p_device_buffer);

	// 深度バッファ用サーフェイスを取得
	m_p_device->
		GetDepthStencilSurface(&m_p_device_depth);
	
	// デバイスにzテクスチャをレンダーターゲットに設定する
	m_p_device->
		SetRenderTarget(0, m_p_tex_suf);
	m_p_device->
		SetDepthStencilSurface(m_p_depth_buffer);

	// beginscene内だった場合、いったん終了させる
	//m_p_device->EndScene();
	
	// テクスチャサーフェイスのクリア
	m_p_device->Clear(0, NULL, D3DCLEAR_TARGET | 
		D3DCLEAR_ZBUFFER, 
		// 背景色も変更
		D3DCOLOR_ARGB(255, 255, 255, 255),
		1.0f, 0);

	//m_p_device->BeginScene();

	CommitTechnique();

	HRESULT hr = m_p_effect->Begin(
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

	HRESULT hr = m_p_effect->End();

	// 深度バッファを元に戻す
	// デバイスに元のサーフェイスを戻す
	m_p_device->SetRenderTarget(0, m_p_device_buffer);
	m_p_device->SetDepthStencilSurface(m_p_device_depth);

	m_p_device_depth = NULL;
	m_p_depth_buffer = NULL;
	
	// GetSurfaceLevelで内部カウンタが+1されているのでReleaseして内部カウンタを減らす
	m_p_device->SetVertexShader(NULL);
	m_p_device->SetPixelShader(NULL);


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

	return m_p_tex;
}