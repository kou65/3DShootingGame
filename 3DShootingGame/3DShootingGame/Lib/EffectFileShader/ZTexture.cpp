#include"ZTexture.h"




ZTexture::ZTexture() {

	// デバイスの取得
	m_p_device =
		Graphics::GetInstance()->GetDevice();

	D3DXMatrixIdentity(&m_world_mat);
	D3DXMatrixIdentity(&m_view_mat);
	D3DXMatrixIdentity(&m_proj_mat);
	m_h_world_mat = NULL;
	m_h_view_mat = NULL;
	m_h_proj_mat = NULL;
	m_h_tech = NULL;
	m_start_pass_num = 0;
}


bool ZTexture::Init(
	const char* effect_file_name,
	UINT z_tex_width,
	UINT z_tex_height,
	D3DFORMAT z_tex_format
) {

	// エフェクトファイル作成
	if (FAILED(D3DXCreateEffectFromFile(
		m_p_device,
		TEXT("ZTexture.fx"),
		NULL,
		NULL,
		0,
		NULL,
		&m_p_effect,
		NULL))) {
		return false;
	}

	// エフェクト内の各種パラメータハンドルを取得
	m_h_world_mat = m_p_effect->
		GetParameterByName(NULL, "matWorld");
	m_h_view_mat = m_p_effect->
		GetParameterByName(NULL, "matView");
	m_h_proj_mat = m_p_effect->
		GetParameterByName(NULL, "matProj");
	m_h_tech = m_p_effect->
		GetTechniqueByName("ZValuePlotTec");


	// zテクスチャ作成
	if (FAILED(
		D3DXCreateTexture(
			m_p_device,
			z_tex_width,
			z_tex_height,
			1,
			D3DUSAGE_RENDERTARGET,
			z_tex_format,
			D3DPOOL_DEFAULT,
			&m_p_tex
		))
		) {
		return false;
	}

	// 深度バッファサーフェイスを取得
	// Zテストを有効にするため
	// これをしないと前後関係がめちゃくちゃになる


	// サーフェイス取得
	m_p_tex->GetSurfaceLevel(0, &m_p_tex_suf);

	IDirect3DSurface9 * p_sur;

	// デプスステンシルレベル取得
	m_p_device->GetDepthStencilSurface(&p_sur);

	// サーフェイスのサイズを取得
	D3DSURFACE_DESC desc;
	p_sur->GetDesc(&desc);
	// 解放
	p_sur->Release();

	// 深度バッファはテクスチャとして作成できない、
	// Zテクスチャと同じ大きさを持つ深度バッファを新規に作成する
	HRESULT hr = m_p_device->CreateDepthStencilSurface(
		z_tex_width,
		z_tex_height,
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


void ZTexture::Begin() {

	if (m_p_effect == nullptr) {
		return;
	}

	// デバイスが持っているバッファを一時保存
	m_p_device->
		GetRenderTarget(0, &m_p_device_buffer);
	m_p_device->
		GetDepthStencilSurface(&m_p_device_depth);
	
	// デバイスにz値テクスチャサーフェイスと深度バッファを設定
	m_p_device->
		SetRenderTarget(0, m_p_tex_suf);
	m_p_device
		->SetDepthStencilSurface(m_p_depth_buffer);
	
	m_p_device->Clear(0, NULL, D3DCLEAR_TARGET | 
		D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 255, 255, 255),
		1.0f, 0);

	m_p_effect->SetTechnique(m_h_tech);

	UINT pass_num = 0;
	HRESULT hr = m_p_effect->Begin(&pass_num, 0);

	if (hr != S_OK) {
		hr = S_OK;
		return;
	}
}

void ZTexture::End() {

	HRESULT hr = m_p_effect->End();

	//// デバイスに元のサーフェイスを戻す
	m_p_device->SetRenderTarget(0, m_p_device_buffer);
	m_p_device->SetDepthStencilSurface(m_p_device_depth);
	//
	//m_p_device_depth = NULL;
	//m_p_depth_buffer = NULL;
	//
	// 固定機能に戻す
	m_p_device->SetVertexShader(NULL);
	m_p_device->SetPixelShader(NULL);

	if (hr != S_OK) {
		hr = S_OK;
		return;
	}
}


void ZTexture::BeginPass() {

	HRESULT hr = m_p_effect->BeginPass(m_start_pass_num);	

	if (hr != S_OK) {
		hr = S_OK;
		return;
	}
}


void ZTexture::EndPass() {

	HRESULT hr = m_p_effect->EndPass();

	if (hr != S_OK) {
		hr = S_OK;
		return;
	}
}


void ZTexture::SetWorldMatrix(D3DXMATRIX* mat) {
	m_world_mat = *mat;
}


void ZTexture::SetViewMatrix(D3DXMATRIX* mat) {
	m_view_mat = *mat;
}


void ZTexture::SetProjMatrix(D3DXMATRIX* mat) {
	m_proj_mat = *mat;
}


bool ZTexture::SetParameter() {

	// マトリックスをセット
	m_p_effect->SetMatrix(m_h_world_mat, &m_world_mat);
	m_p_effect->SetMatrix(m_h_view_mat, &m_view_mat);
	m_p_effect->SetMatrix(m_h_proj_mat, &m_proj_mat);


	return true;
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