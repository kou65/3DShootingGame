#include"../DirectionalLight/Light.h"




// 初期化
void LightShader::Init() {

	// エフェクトファイル生成,// デクル初期化
	ShaderBase::CreateEffectFile(
		"Lib/Shader/EffectFile/Light.fx",
		"tech1",
		VertexDecl::OBJ
	);

	// 変換のハンドル初期化
	StandardTSShader::InitTSHandle(
	"g_world",
		"g_view",
		"g_proj"
	);

	// 各シェーダーパラメータ名代入
	m_dir_light_name = "g_dir_light";
	m_point_light_pos_name = "g_point_light_pos";
	m_light_color_name = "g_light_color";
	m_eye_dir_name = "g_eye_dir";
	m_eye_pos_name = "g_eye_pos";

	// 鏡面
	m_specular_name = "g_specular";
	m_specular_power_name = "g_specular_power";

	// アンビエント
	m_ambient_name = "g_ambient";

	// テクスチャ名
	m_tex_name = "g_tex";

	// カラー名
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
	m_light_data.point_light.pos = D3DXVECTOR4(10.f, 10.f, 0.f, 0.f);

	mp_tex = nullptr;
}


// 更新
void  LightShader::Update() {

	// 更新
	ShaderBase::UpdateDecl();

	// 更新
	StandardTSShader::UpdateStandardCamera();
	StandardTSShader::Update();

	// ライトセット
	UpdateLight();

	// コミット
	ShaderBase::CommitShader();
}


void LightShader::InitHandle() {

	// ハンドル取得
	m_h_dir_light =
		mp_effect->GetParameterByName(NULL,m_dir_light_name.c_str());
	m_h_point_light_pos = 
		mp_effect->GetParameterByName(NULL, m_point_light_pos_name.c_str());
	m_h_light_color =
		mp_effect->GetParameterByName(NULL, m_light_color_name.c_str());
	m_h_eye_dir = 
		mp_effect->GetParameterByName(NULL, m_eye_dir_name.c_str());

	// ライト設定ハンドル取得
	m_h_specular =
		mp_effect->GetParameterByName(NULL, m_specular_name.c_str());
	m_h_specular_power =
		mp_effect->GetParameterByName(NULL, m_specular_power_name.c_str());
	m_h_ambient =
		mp_effect->GetParameterByName(NULL, m_ambient_name.c_str());


	// ライトビュー
	m_h_light_view = 
		mp_effect->GetParameterByName(NULL, m_light_view_name.c_str());

	// ライト射影
	m_h_light_proj = 
		mp_effect->GetParameterByName(NULL, m_light_proj_name.c_str());

	// テクスチャとカラー
	m_h_tex = 
		mp_effect->GetParameterByName(NULL, m_tex_name.c_str());
	m_h_color = 
		mp_effect->GetParameterByName(NULL, m_color_name.c_str());
}


void LightShader::UpdateLight() {

	// ライトに情報を送る

	// 方向ライト
	mp_effect->SetVector(m_h_dir_light,
		&m_light_data.direction);

	// ライトカラー
	mp_effect->SetVector(m_h_light_color,
		&m_light_data.light_color);

	// 点光源ライト
	mp_effect->SetVector(m_h_point_light_pos,
		&m_light_data.point_light.pos);

	// 注視点座標
	mp_effect->SetVector(m_h_eye_dir,
		&m_light_data.eye_direction);

	// 環境光
	mp_effect->SetVector(m_h_ambient,
		&m_light_data.material.ambient);

	// ディフューズカラー
	mp_effect->SetVector(m_h_color,
		&m_color);

	// テクスチャ
	mp_effect->SetTexture(m_h_tex,
		mp_tex);
}



void LightShader::SetLightData(const LightData&light_data) {
	m_light_data = light_data;
}


void LightShader::SetLightView(const D3DXMATRIX&light_view) {
	m_light_view_mat = light_view;
}


void LightShader::SetLightProj(const D3DXMATRIX&light_proj) {
	m_light_proj_mat = light_proj;
}


void LightShader::SetTexture(IDirect3DTexture9*tex) {
	mp_tex = tex;
}


void LightShader::SetColor(const D3DXVECTOR4 &color) {
	m_color = color;
}


void LightShader::SetShadowTexture(IDirect3DTexture9*tex) {
	mp_shadow_map_tex = tex;
}
