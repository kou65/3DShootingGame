#include"../DirectionalLight/DirectionalLight.h"




// 初期化
void Light::Init() {

	// エフェクトファイル生成,// デクル初期化
	ShaderBase::CreateEffectFile(
		"Lib/Shaderfile/DirectionalLight.fx",
		"tech1",
		VertexDecl::OBJ
	);

	// パラメータ名
	StandardTransformShader::m_world_mat_name = "g_world";
	StandardTransformShader::m_view_mat_name = "g_view";
	StandardTransformShader::m_proj_mat_name = "g_proj";

	// 変換のハンドル初期化
	StandardTransformShader::InitTransformHandle();

	// 各シェーダーパラメータ名代入
	m_dir_light_name = "g_dir_light";
	m_tex_name = "g_tex";
	m_color_name = "g_color";
	m_point_light_pos_name = "g_point_light_pos";
	m_light_color_name = "g_light_color";
	m_eye_dir_name = "g_eye_dir";
	m_eye_pos_name = "g_eye_pos";

	// ライトハンドル初期化
	InitHandle();

	// カラー初期化
	m_color.x = 1.f;
	m_color.y = 1.f;
	m_color.z = 1.f;
	m_color.w = 1.f;

	// ライトカラー初期化
	m_light_color = m_color;

	// 各パラメータ初期化
	m_p_tex = nullptr;
	m_dir_light = D3DXVECTOR4(10.f,10.f,0.f,0.f);
	m_point_light_pos = D3DXVECTOR4(10.f, 10.f, 0.f, 0.f);
}


// 更新
void  Light::Update() {

	// 更新
	ShaderBase::UpdateDecl();

	// 更新
	StandardTransformShader::UpdateStandardCamera();
	StandardTransformShader::UpdateTransfromMatrix();

	// ライトセット
	UpdateLight();

	// コミット
	ShaderBase::CommitShader();
}


void Light::InitHandle() {

	// ハンドル取得
	m_h_dir_light =
		m_p_effect->GetParameterByName(NULL,m_dir_light_name.c_str());
	m_h_point_light_pos = 
		m_p_effect->GetParameterByName(NULL, m_point_light_pos_name.c_str());
	m_h_light_color =
		m_p_effect->GetParameterByName(NULL, m_light_color_name.c_str());
	m_h_eye_dir = 
		m_p_effect->GetParameterByName(NULL, m_eye_dir_name.c_str());

	m_h_tex =
		m_p_effect->GetParameterByName(NULL, m_tex_name.c_str());
	m_h_color =
		m_p_effect->GetParameterByName(NULL, m_color_name.c_str());
}


void Light::UpdateLight() {

	// ライトに情報を送る

	// 方向ライト
	m_p_effect->SetVector(m_h_dir_light, &m_dir_light);

	// ライトカラー
	m_p_effect->SetVector(m_h_light_color, &m_light_color);

	// 点光源ライト
	m_p_effect->SetVector(m_h_point_light_pos, &m_point_light_pos);

	// 注視点座標
	m_p_effect->SetVector(m_h_eye_dir,&m_eye_dir);

	// ディフューズカラー
	m_p_effect->SetVector(m_h_color, &m_color);
	// テクスチャ
	m_p_effect->SetTexture(m_h_tex, m_p_tex);	
}


void Light::SetDirLight(const D3DXVECTOR4&vec) {
	m_dir_light = vec;
}


void Light::SetPointLight(const D3DXVECTOR4&pos) {
	m_point_light_pos = pos;
}


void Light::SetTexture(IDirect3DTexture9*tex) {
	m_p_tex = tex;
}


void Light::SetColor(D3DXVECTOR4 &color) {
	m_color = color;
}


void Light::SetLightColor(D3DXVECTOR4&light_color) {
	m_light_color = light_color;
}


void Light::SetEyeDir(D3DXVECTOR4&vec4) {
	m_eye_dir = vec4;
}


void Light::SetEyePos(D3DXVECTOR4&eye_pos) {
	m_eye_pos = eye_pos;
}
