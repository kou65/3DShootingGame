#include"BlurFilter.h"



BlurFilter::BlurFilter(
	const int &width,
	const int &height,
	const VertexDecl::Type&type
) {

	// バックバッファの解像度代入
	m_width = width;
	m_height = height;

	// 初期化
	Init(type);

	// デバイス取得
	mp_device = Graphics::GetInstance()->GetDevice();
}


void BlurFilter::Init(
	const VertexDecl::Type&type
) {

	// シェーダーファイル作成
	ShaderBase::CreateEffectFile(
		"Lib/Shader/EffectFile/BlurFilter.fx",
		"TShader",
		type
	);

	// ハンドル取得
	m_h_tu =
	mp_effect->GetParameterByName(NULL, "g_tu");
	m_h_tv =
	mp_effect->GetParameterByName(NULL, "g_tv");

	// テクスチャハンドル取得
	m_h_tex =
	mp_effect->GetParameterByName(NULL, "g_tex");

	// 変換ハンドル
	m_h_world =
		mp_effect->GetParameterByName(NULL, "g_world");
	m_h_view =
		mp_effect->GetParameterByName(NULL, "g_view");
	m_h_proj = 
		mp_effect->GetParameterByName(NULL, "g_proj");


	// 1テクセルの大きさをセット
	float texel_u = 1.f / m_width;
	float texel_v = 1.f / m_height;

	// テクセル数配列
	float u[ARRAY];
	float v[ARRAY];

	// 割合ごとに配列分入れていく
	for (int i = 0; i < ARRAY; i++) {

		u[i] = texel_u * (i + 1);
		v[i] = texel_v * (i + 1);
	}

	// gpuコミット
	mp_effect->SetFloatArray(m_h_tu, u, ARRAY);
	mp_effect->SetFloatArray(m_h_tv, v, ARRAY);


	// バックバッファサーフェイス作成
	for (int i = 0; i < SURFACE_VALUE; i++) {
		m_bf_surface_list[i].CraeteDepthSurface();
	}

}


void BlurFilter::Update() {


	D3DCAPS9 caps;

	// バックバッファの数を取得
	mp_device->GetDeviceCaps(&caps);

	// バージョンが2.0以上対応か
	if (caps.VertexShaderVersion >= D3DVS_VERSION(2, 0) &&
		caps.VertexShaderVersion >= D3DVS_VERSION(2, 0)) {

	}
	else {
		return;
	}
}


void BlurFilter::Begin(
	UINT &total_pass_num,
	const DWORD &device_state_num
) {

	// 描画の開始
	mp_effect->Begin(
		&total_pass_num,
		device_state_num
	);
}


void BlurFilter::BeginPass(
	const int &out_pass_num
) {

	// パスの開始
	mp_effect->BeginPass(out_pass_num);
}


void BlurFilter::End() {

	// 描画終了
	mp_effect->End();

	// レンダーターゲットをセットする必要がある
}


void BlurFilter::SetTexture(IDirect3DTexture9*p_tex) {

	mp_effect->SetTexture(m_h_tex, p_tex);
}


void BlurFilter::SetTransform(
	const D3DXMATRIX&world_mat,
	const D3DXMATRIX&view_mat,
	const D3DXMATRIX&proj_mat
) {

	// セット
	mp_effect->SetMatrix(m_h_world, &world_mat);
	mp_effect->SetMatrix(m_h_view, &view_mat);
	mp_effect->SetMatrix(m_h_proj, &proj_mat);
}
