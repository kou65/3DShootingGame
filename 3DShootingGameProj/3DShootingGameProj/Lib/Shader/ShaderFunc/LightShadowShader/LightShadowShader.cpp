#include"LightShadowShader.h"




void LightShadowShader::Init(const VertexDecl::Type&type) {


	// エフェクトファイル作成
	CreateEffectFile(
		"Lib/Shader/EffectFile/DepthShadow.fx",
		"DepthBufferShadowTec",
		type
	);

	// ハンドルの初期化
	InitHandle();

}


void LightShadowShader::Update() {

	// シェーダーの更新
	StandardTSShader::Update();
	//ColorShader::UpdateColor();
	Light::UpdateLight();

	// 影データ送信
	CommitShadowData();
}


void LightShadowShader::CommitShadowData() {

	mp_effect->SetMatrix(
		m_h_light_view,
		&m_shadow_data.light_view_mat
	);

	mp_effect->SetMatrix(
		m_h_light_proj,
		&m_shadow_data.light_proj_mat
	);

}



void LightShadowShader::InitHandle() {


	// ハンドル初期化
	StandardTSShader::InitTSHandle(
		"g_world",
		"g_view",
		"g_proj"
	);

	// カラーシェーダー
	ColorShader::InitColorHandle(
		"g_color",
		"g_tex"
	);

	// マテリアルハンドル
	Light::InitMateHandle(
		"g_mt_ambient",
		"g_mt_specular",
		"g_mt_diffuse",
		"g_ambient"
	);

	// カメラハンドル
	Light::InitCameraHandle(
		"g_eye_pos",
		"g_eye_dir"
	);

	// ライト方向ハンドル
	Light::InitLightDirHandle(
		"g_light_dir",
		"g_light_color"
	);

	// 点光源ハンドル
	Light::InitPointLightHandle(
		"g_pl_pos",
		"g_pl_diffuse",
		"g_pl_specular",
		"g_pl_attenuate"
	);

	InitShadowHandle();
}


void LightShadowShader::InitShadowHandle() {

	m_h_light_view =
	mp_effect->GetParameterByName(
		NULL,
		"g_light_view"
	);

	m_h_light_proj =
	mp_effect->GetParameterByName(
		NULL,
		"g_light_proj"
	);

	m_h_shadow_map =
	mp_effect->GetParameterByName(
		NULL,
		"g_shadow_tex"
	);
}


void LightShadowShader::SetShadowData(const ShadowData&shadow_data) {
	m_shadow_data = shadow_data;
}


void LightShadowShader::SetShadowMap(IDirect3DTexture9*p_tex) {
	mp_effect->SetTexture(m_h_shadow_map, p_tex);
}
