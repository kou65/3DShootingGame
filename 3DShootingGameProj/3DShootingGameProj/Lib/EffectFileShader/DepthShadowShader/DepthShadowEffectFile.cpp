#include"DepthShadowEffectFile.h"



DepthShadowShader::
DepthShadowShader() {

	m_p_graphics = Graphics::GetInstance();


	// エフェクトファイル作成
	CreateEffectFile(
		"Lib/Shaderfile/DepthShadow.fx",
		"DepthBufferShadowTec",
		VertexDecl::OBJ
	);

	// ハンドル初期化
	StandardTransformShader::InitTransformHandle(
		"g_world",
		"g_view",
		"g_proj"
	);
}


void DepthShadowShader::Init() {


	m_h_light_view_mat = 
		m_p_effect->GetParameterByName(NULL, "g_light_view");
	m_h_light_proj_mat = 
		m_p_effect->GetParameterByName(NULL, "g_light_proj");
	m_h_shadow_map_tex = 
		m_p_effect->GetParameterByName(NULL, "g_shadow_tex");
	
	// ハンドル
	if (!m_h_light_view_mat || !m_h_light_proj_mat || !m_h_shadow_map_tex) {
		return;
	}
}

bool DepthShadowShader::SetShandowMap(
	IDirect3DTexture9*shadow_map) {

	// テクスチャ
	m_p_shadow_map_tex = shadow_map;

	return true;
}


void DepthShadowShader::SetLightViewMatrix(const D3DXMATRIX&mat) {
	m_mat_light_view = mat;
}


void DepthShadowShader::SetLightProjMatrix(const D3DXMATRIX&mat) {
	m_mat_light_proj = mat;
}


void DepthShadowShader::Update() {

	// 更新
	ShaderBase::UpdateDecl();

	// 更新
	StandardTransformShader::UpdateTransfromMatrix();

	// パラメータ更新
	SetParamToEffect();

	m_p_effect->CommitChanges();
}


// 描画の開始を宣言する
void DepthShadowShader::Begin(
	UINT &total_pass_num,
	const DWORD &device_state_num) {


	CommitTechnique();

	m_p_effect->Begin(&total_pass_num, device_state_num);

}


// 描画の終了を宣言する
void DepthShadowShader::
End() {

	m_p_effect->End();

	// 固定機能に戻す
	m_p_graphics->GetInstance()->GetDevice()->SetVertexShader(NULL);
	m_p_graphics->GetInstance()->GetDevice()->SetPixelShader(NULL);
}


// 登録されているパラメータ情報をエフェクトにセット
bool DepthShadowShader::
SetParamToEffect() {

	// パラメータセット
	m_p_effect->SetMatrix(m_h_light_view_mat, &m_mat_light_view);
	m_p_effect->SetMatrix(m_h_light_proj_mat, &m_mat_light_proj);

	HRESULT hr = m_p_effect->
		SetTexture(m_h_shadow_map_tex, m_p_shadow_map_tex);

	return true;
}
