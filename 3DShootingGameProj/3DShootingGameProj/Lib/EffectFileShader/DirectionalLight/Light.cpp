#include"../DirectionalLight/Light.h"




// 初期化
void Light::Init() {

	// エフェクトファイル生成,// デクル初期化
	ShaderBase::CreateEffectFile(
		"Lib/Shaderfile/Shade.fx",
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
	m_point_light_pos_name = "g_point_light_pos";
	m_light_color_name = "g_light_color";
	m_eye_dir_name = "g_eye_dir";
	m_eye_pos_name = "g_eye_pos";

	m_tex_name = "g_tex";
	m_color_name = "g_color";

	// ライトビュー
	m_light_view_name = "g_light_view";
	// ライト射影
	m_light_proj_name = "g_light_proj";
	// シャドウマップ
	m_shadow_tex_name = "g_shadow_tex";

	// ライトハンドル初期化
	InitHandle();

	// カラー初期化
	m_color.x = 1.f;
	m_color.y = 1.f;
	m_color.z = 1.f;
	m_color.w = 1.f;

	// ライトカラー初期化
	m_light_data.light_color = m_color;

	// 各パラメータ初期化
	m_light_data.direction = D3DXVECTOR4(10.f, 10.f, 0.f, 0.f);
	m_light_data.point_light_pos = D3DXVECTOR4(10.f, 10.f, 0.f, 0.f);

	m_p_tex = nullptr;
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

	// ライトビュー
	m_h_light_view = 
		m_p_effect->GetParameterByName(NULL, m_light_view_name.c_str());

	// ライト射影
	m_h_light_proj = 
		m_p_effect->GetParameterByName(NULL, m_light_proj_name.c_str());

	// テクスチャとカラー
	m_h_tex =
		m_p_effect->GetParameterByName(NULL, m_tex_name.c_str());
	m_h_color =
		m_p_effect->GetParameterByName(NULL, m_color_name.c_str());
}


void Light::UpdateLight() {

	// ライトに情報を送る

	// 方向ライト
	m_p_effect->SetVector(m_h_dir_light, &m_light_data.direction);

	// ライトカラー
	m_p_effect->SetVector(m_h_light_color, &m_light_data.light_color);

	// 点光源ライト
	m_p_effect->SetVector(m_h_point_light_pos, &m_light_data.point_light_pos);

	// 注視点座標
	m_p_effect->SetVector(m_h_eye_dir,&m_light_data.eye_dir);

	// ディフューズカラー
	m_p_effect->SetVector(m_h_color, &m_color);
	// テクスチャ
	m_p_effect->SetTexture(m_h_tex, m_p_tex);	
}



void Light::SetLightData(const LightData&light_data) {
	m_light_data = light_data;
}


void Light::SetLightView(const D3DXMATRIX&light_view) {
	m_light_view_mat = light_view;
}


void Light::SetLightProj(const D3DXMATRIX&light_proj) {
	m_light_proj_mat = light_proj;
}


void Light::SetTexture(IDirect3DTexture9*tex) {
	m_p_tex = tex;
}


void Light::SetColor(const D3DXVECTOR4 &color) {
	m_color = color;
}


void Light::SetShadowTexture(IDirect3DTexture9*tex) {
	m_p_shadow_map_tex = tex;
}


