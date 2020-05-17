#include"DepthShadowEffectFile.h"



DepthShadowShader::DepthShadowShader(
const VertexDecl::Type&type
) {

	mp_graphics = Graphics::GetInstance();


	// エフェクトファイル作成
	CreateEffectFile(
		"Lib/Shader/EffectFile/DepthShadow.fx",
		"DepthBufferShadowTec",
		type
	);

	// ハンドル初期化
	StandardTSShader::InitTransformHandle(
		"g_world",
		"g_view",
		"g_proj"
	);
}


void DepthShadowShader::Init() {


	// ハンドル名取得
	m_h_light_view = 
		mp_effect->GetParameterByName(NULL, "g_light_view");
	m_h_light_proj = 
		mp_effect->GetParameterByName(NULL, "g_light_proj");
	m_h_shadow_map_tex = 
		mp_effect->GetParameterByName(NULL, "g_shadow_tex");
	
	// ハンドル
	if (!m_h_light_view || !m_h_light_proj || !m_h_shadow_map_tex) {
		return;
	}

	// 一回更新しておく
	Update();
}


bool DepthShadowShader::SetShandowMap(
	IDirect3DTexture9*shadow_map) {

	// テクスチャ
	mp_shadow_map_tex = shadow_map;

	return true;
}


void DepthShadowShader::CommitDeviceViewProj() {
	StandardTSShader::UpdateStandardCamera();
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

	// パラメータ更新
	SetParamToEffect();

	// コミット
	//mp_effect->CommitChanges();
}


// 描画の開始を宣言する
void DepthShadowShader::Begin(
	UINT &total_pass_num,
	const DWORD &device_state_num) {

	//m_cpDev->Clear( 0, NULL, D3DCLEAR_TARGET 
//	| D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255,255,255,255),
//	1.0f, 0 );

	mp_effect->SetTechnique(m_h_technique);

	mp_effect->Begin(&total_pass_num, device_state_num);
}


void DepthShadowShader::End() {

	mp_effect->End();

	// 固定機能に戻す
	mp_graphics->GetInstance()->GetDevice()->SetVertexShader(NULL);
	mp_graphics->GetInstance()->GetDevice()->SetPixelShader(NULL);
}


bool DepthShadowShader::SetParamToEffect() {

	// パラメータセット
	mp_effect->SetMatrix(m_h_world, &m_mat_world);
	mp_effect->SetMatrix(m_h_light_view, &m_mat_light_view);
	mp_effect->SetMatrix(m_h_light_proj, &m_mat_light_proj);
	mp_effect->SetMatrix(m_h_view_mat, &m_mat_view);
	mp_effect->SetMatrix(m_h_proj_mat, &m_mat_proj);
	

	HRESULT hr = mp_effect->
		SetTexture(m_h_shadow_map_tex, mp_shadow_map_tex);

	return true;
}
