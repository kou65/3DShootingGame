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
	StandardTSShader::InitTSHandle(
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
	m_h_shadow_map = 
		mp_effect->GetParameterByName(NULL, "g_shadow_tex");
	m_h_tex =
		mp_effect->GetParameterByName(NULL, "g_tex");


	m_h_light_dir =
		mp_effect->GetParameterByName(NULL, "g_dir_light");
	m_h_specular =
		mp_effect->GetParameterByName(NULL, "g_specular");
	m_h_specular_power =
		mp_effect->GetParameterByName(NULL, "g_specular_power");
	m_h_ambient =
		mp_effect->GetParameterByName(NULL, "g_ambient");
	m_h_eye_dir =
		mp_effect->GetParameterByName(NULL, "g_eye_dir");
	m_h_eye_pos =
		mp_effect->GetParameterByName(NULL, "g_eye_pos");
	
	// ハンドル
	if (!m_h_light_view || !m_h_light_proj || !m_h_shadow_map) {
		return;
	}

	// 一回更新しておく
	Update();
}


bool DepthShadowShader::SetShandowMap(
	IDirect3DTexture9*shadow_map) {

	// テクスチャ
	mp_shadow_map = shadow_map;

	return true;
}


bool DepthShadowShader::SetTexture(IDirect3DTexture9*p_tex) {

	if (mp_effect->SetTexture(m_h_tex, p_tex) == S_OK) {
		return true;
	}

	return false;
}


void DepthShadowShader::CommitDeviceViewProj() {
	StandardTSShader::UpdateStandardCamera();
}


void DepthShadowShader::Update() {

	// 更新
	ShaderBase::UpdateDecl();

	// パラメータ更新
	SetParamToEffect();

	// コミット
	mp_effect->CommitChanges();
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

	// テクスチャ
	HRESULT hr = mp_effect->
		SetTexture(m_h_shadow_map, mp_shadow_map);

	// ライトパラメータセット
	mp_effect->SetVector(m_h_ambient,
		&m_light_data.material.ambient
	);

	mp_effect->SetVector(m_h_eye_dir,
		&m_light_data.eye_direction
	);

	mp_effect->SetVector(m_h_eye_pos,
		&m_light_data.eye_pos
	);

	mp_effect->SetVector(
		m_h_light_dir,
		&m_light_dir
	);


	return true;
}


void DepthShadowShader::SetLightViewMatrix(const D3DXMATRIX&mat) {
	m_mat_light_view = mat;
}


void DepthShadowShader::SetLightProjMatrix(const D3DXMATRIX&mat) {
	m_mat_light_proj = mat;
}


void DepthShadowShader::SetLightData(const LightData&data) {
	m_light_data = data;
}
