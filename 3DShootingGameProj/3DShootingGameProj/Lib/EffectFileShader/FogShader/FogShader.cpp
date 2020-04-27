#include"FogShader.h"




void FogShader::Init() {
	// シェーダーファイル生成
	if (CreateEffectFile(
		"Lib/ShaderFile/ObjDraw.fx",
		"tech1",
		VertexDecl::OBJ
	) == false) {
		return;
	}

	// 白
	SetColor(D3DXVECTOR4(1.f, 1.f, 1.f, 1.f));

	// トランスフォームハンドル取得
	StandardTransformShader::InitTransformHandle(
		"g_world",
		"g_view",
		"g_proj"
	);

	m_light_dir_name = "g_light_dir";
	m_ambient_name = "g_ambient";
	m_color_name = "g_fog_color";
	m_texture_name = "g_tex";
	m_near_name = "g_near";
	m_far_name = "g_far";
	m_eye_pos_name = "g_eye_pos";

	// ハンドル初期化
	InitHandle();
}



void FogShader::InitHandle() {

	// ハンドル取得
	m_h_light_dir =
		m_p_effect->GetParameterByName(NULL,m_light_dir_name.c_str());
	m_h_color =
		m_p_effect->GetParameterByName(NULL, m_color_name.c_str());
	m_h_ambient = 
		m_p_effect->GetParameterByName(NULL, m_ambient_name.c_str());
	m_h_texture = 
		m_p_effect->GetParameterByName(NULL, m_texture_name.c_str());
	m_h_near =
		m_p_effect->GetParameterByName(NULL, m_near_name.c_str());
	m_h_far =
		m_p_effect->GetParameterByName(NULL, m_far_name.c_str());
	m_h_eye_pos =
		m_p_effect->GetParameterByName(NULL, m_eye_pos_name.c_str());
}


void FogShader::Update() {


	// 更新
	ShaderBase::UpdateDecl();

	// 更新
	StandardTransformShader::UpdateStandardCamera();
	StandardTransformShader::UpdateTransfromMatrix();



	// コミット
	ShaderBase::CommitShader();
}


void FogShader::UpdateFog() {


	// 方向ライト
	m_p_effect->SetVector(m_h_light_dir, &m_light_data.direction);

	// カラー
	m_p_effect->SetVector(m_h_color, &m_color);

	// 注視点座標
	m_p_effect->SetVector(m_h_eye_pos, &m_light_data.eye_pos);

	// 環境カラー
	m_p_effect->SetVector(m_h_ambient, &m_light_data.ambient_color);

	// テクスチャ
	m_p_effect->SetTexture(m_h_texture, m_p_tex);

	m_p_effect->SetFloat(m_h_far, m_far);
	m_p_effect->SetFloat(m_h_near, m_near);
}


void FogShader::SetColor(const D3DXVECTOR4 &color) {
	m_color = color;
}


void FogShader::SetTexture(IDirect3DTexture9* p_tex) {
	// テクスチャセット
	HRESULT hr = m_p_effect->SetTexture(m_h_texture, p_tex);
	m_p_tex = p_tex;
}


void FogShader::SetLight(const LightData&data) {
	m_light_data = data;
}


void FogShader::SetNear(const float&num) {
	m_near = num;
}


void FogShader::SetFar(const float&num) {
	m_far = num;
}
