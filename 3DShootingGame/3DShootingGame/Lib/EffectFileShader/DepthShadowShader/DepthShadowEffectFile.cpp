#include"DepthShadowEffectFile.h"



DepthShadowEffectFile::
DepthShadowEffectFile() {

	m_p_graphics = Graphics::GetInstance();
}

bool DepthShadowEffectFile::
Init(const char*file_name) {

	if (FAILED(D3DXCreateEffectFromFile(
		m_p_graphics->GetInstance()->GetDevice(),
		TEXT(file_name),
		NULL,
		NULL,
		0,
		NULL,
		&m_p_effect,
		NULL))) {
		return false;
	}

	// ハンドル取得
	m_h_mat_world = 
		m_p_effect->
		GetParameterByName(NULL, "mat_world");
	m_h_camera_view_mat 
		= m_p_effect->
		GetParameterByName(NULL, "mat_camera_view");
	m_h_camera_proj_mat = 
		m_p_effect->GetParameterByName(NULL, "mat_camera_proj");
	m_h_light_view_mat = 
		m_p_effect->GetParameterByName(NULL, "mat_light_view");
	m_h_light_proj_mat = 
		m_p_effect->GetParameterByName(NULL, "mat_light_proj");
	m_h_shadow_map_tex = 
		m_p_effect->GetParameterByName(NULL, "shadow_tex");
	m_h_technique = 
		m_p_effect->GetTechniqueByName("DepthBufferShadowTec");

	// ハンドル
	if (!m_h_mat_world || !m_h_camera_view_mat || !m_h_camera_proj_mat
		|| !m_h_light_view_mat || !m_h_light_proj_mat || !m_h_shadow_map_tex
		|| !m_h_technique)
		return false;

	return true;
}

bool DepthShadowEffectFile::
SetShandowMap(
	IDirect3DTexture9*shadow_map) {

	if (shadow_map == NULL) {
		return false;
	}

	// テクスチャ
	m_p_shadow_map_tex = shadow_map;

	return true;
}


void DepthShadowEffectFile::
SetWorldMatrix(
	D3DXMATRIX*mat
) {
	m_mat_world = *mat;
}

void DepthShadowEffectFile::
SetCameraViewMatrix(D3DXMATRIX*mat) {
	m_mat_camera_view = *mat;
}

void DepthShadowEffectFile::
SetCameraProjMatrix(D3DXMATRIX*mat) {
	m_mat_camera_proj = *mat;
}

void DepthShadowEffectFile::
SetLightViewMatrix(D3DXMATRIX*mat) {
	m_mat_light_view = *mat;
}

void DepthShadowEffectFile::
SetLightProjMatrix(D3DXMATRIX*mat) {
	m_mat_light_proj = *mat;
}


// 描画の開始を宣言する
HRESULT DepthShadowEffectFile::
Begin() {

	// 各サーフェイス初期化
	m_p_graphics->GetInstance()->GetDevice()
		->Clear(0, NULL, D3DCLEAR_TARGET 
			| D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(200, 0, 200, 200), 1.0f, 0);

	m_p_effect->SetTechnique(m_h_technique);

	m_p_effect->Begin(0, 0);

	return S_OK;
}

// 描画の終了を宣言する
HRESULT DepthShadowEffectFile::
End() {

	m_p_effect->End();

	// 固定機能に戻す
	m_p_graphics->GetInstance()->GetDevice()->SetVertexShader(NULL);
	m_p_graphics->GetInstance()->GetDevice()->SetPixelShader(NULL);

	return S_OK;
}

// パスの開始を宣言する
HRESULT DepthShadowEffectFile::
BeginPass() {

	m_p_effect->BeginPass(0);
	return S_OK;
}

// パスの終了を宣言する
HRESULT DepthShadowEffectFile::
EndPass() {
	m_p_effect->EndPass();
	return S_OK;
}

// 登録されているパラメータ情報をエフェクトにセット
bool DepthShadowEffectFile::
SetParamToEffect() {

	// パラメータセット
	m_p_effect->SetMatrix(m_h_mat_world, &m_mat_world);
	m_p_effect->SetMatrix(m_h_camera_view_mat, &m_mat_camera_view);
	m_p_effect->SetMatrix(m_h_camera_proj_mat, &m_mat_camera_proj);
	m_p_effect->SetMatrix(m_h_light_view_mat, &m_mat_light_view);
	m_p_effect->SetMatrix(m_h_light_proj_mat, &m_mat_light_proj);

	HRESULT hr = m_p_effect->
		SetTexture(m_h_shadow_map_tex, m_p_shadow_map_tex);

	return true;
}
