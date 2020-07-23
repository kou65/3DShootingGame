#include"SoftShadow.h"




SoftShadow::SoftShadow(
	const VertexDecl::Type&type
) {

	// エフェクトファイル作成
	CreateEffectFile(
		"Lib/Shader/EffectFile/SoftShadow.hlsl",
		"tech",
		type
	);

	// ハンドル初期化
	StandardTSShader::InitTSHandle(
		"g_world",
		"g_view",
		"g_proj"
	);

	// ハンドル初期化
	InitHandle();


	// GPUに行き、転置されることを考えて逆転置変換行列
	m_t = D3DXMATRIX(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f
	);

	// Z値テクスチャ描画用のスプライト生成
	D3DXCreateSprite(
		Graphics::GetInstance()->GetDevice(),
		&mp_sprite
	);

	m_r_index = 0; 
	RenderTargetData data;
	data.width = 1024;
	data.height = 1024;

	for (int i = 0; i < MAX_RENDER_INDEX; i++) {

		// サーフェイス作成
		m_rt[i].CreateSurface(data);
	}
}


void SoftShadow::InitHandle() {


	// ハンドル名取得
	m_h_shadow_map = 
		mp_effect->GetParameterByName(NULL, "g_shadow_map");
	m_h_model_tex =
		mp_effect->GetParameterByName(NULL, "g_model_tex");

	// ライト系
	m_h_light_dir =
		mp_effect->GetParameterByName(NULL, "light_dir");
	m_h_ambient =
		mp_effect->GetParameterByName(NULL, "g_ambient");
	m_h_eye_dir =
		mp_effect->GetParameterByName(NULL, "g_eye_dir");
	m_h_eye_pos =
		mp_effect->GetParameterByName(NULL, "g_eye_pos");

	// mapsize
	m_h_map_size =
		mp_effect->GetParameterByName(NULL, "MAP_SIZE");

	// 変換用
	m_h_wvp =
		mp_effect->GetParameterByName(NULL, "WVP");

	m_h_wlp =
		mp_effect->GetParameterByName(NULL, "WLP");

	m_h_wvpb =
		mp_effect->GetParameterByName(NULL, "WVPB");

}


void SoftShadow::Update() {

	m_decl.Set();

	// wvp行列更新
	m_wvp = m_mat_world * m_mat_view * m_mat_proj;

	// ライトの空間への射影行列
	m_wlp = m_mat_world * m_light_view * m_light_proj;

	// テクスチャ空間への射影行列
	m_wvpb = m_wlp * m_t;

	// 変換をセット
	mp_effect->SetMatrix(m_h_wvp, &m_wvp);
	mp_effect->SetMatrix(m_h_wlp, &m_wlp);
	mp_effect->SetMatrix(m_h_wvpb, &m_wvpb);

	// セット
	mp_effect->SetFloat(m_h_map_size, m_map_size);

	mp_effect->SetTechnique(m_h_technique);

	// テクスチャセット
	mp_effect->SetTexture(m_h_model_tex,mp_model_tex);
	mp_effect->SetTexture(m_h_shadow_map,mp_shadow_map);
}


void SoftShadow::Begin(
	UINT &total_pass_num,
	const DWORD &device_state_num
) {

	//m_mrt.SetBackBuffer();
	//m_mrt.SetRender(m_rt[m_r_index]);

	HRESULT hr = mp_effect->Begin(
		&total_pass_num,
		device_state_num
	);

	if (hr != S_OK) {
		hr = S_OK;
		return;
	}
}

void SoftShadow::End() {

	// 後始末
	// ちゃんと後始末しないとカウンタが狂って解放されない

	HRESULT hr = mp_effect->End();

	//m_mrt.GetBackBuffer();


	if (hr != S_OK) {
		hr = S_OK;
		return;
	}
}


void SoftShadow::DrawTex(LPDIRECT3DTEXTURE9 tex) {

	if (tex == nullptr) {
		tex = m_rt[m_r_index].GetTexture();
	}

	float Ratio = 1.f;
	//////////////////////////////////
	//■パス3 : Z値テクスチャを描画
	D3DXMATRIX SpriteScaleMat;
	D3DXMatrixScaling(&SpriteScaleMat, Ratio / 3, Ratio / 3, 1.0f);
	mp_sprite->SetTransform(&SpriteScaleMat);
	mp_sprite->Begin(0);
	mp_sprite->Draw(tex, NULL, NULL, NULL, 0xffffffff);
	mp_sprite->End();

}


void SoftShadow::InitAndCreateSurface() {

	m_rt[m_r_index].DestorySurface();

	RenderTargetData data;
	data.width = 1024;
	data.height = 1024;

	m_rt[m_r_index].CreateSurface(data);
}


void SoftShadow::SetLightView(const D3DXMATRIX&mat) {
	m_light_view = mat;
}

void SoftShadow::SetLightProj(const D3DXMATRIX&mat) {
	m_light_proj = mat;
}

void SoftShadow::SetModelTex(LPDIRECT3DTEXTURE9&tex) {
	mp_model_tex = tex;
}

void SoftShadow::SetShadowMap(LPDIRECT3DTEXTURE9&tex) {
	mp_shadow_map = tex;
}

void SoftShadow::SetMapSize(const float &size) {
	m_map_size = size;
}

LPDIRECT3DTEXTURE9 SoftShadow::GetTexture() {
	return m_rt[m_r_index].GetTexture();
}

void SoftShadow::SetRenderIndex(const int&index) {
	m_r_index = (index % MAX_RENDER_INDEX);
}