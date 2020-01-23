#include"EffectFile.h"



EffectFile::EffectFile() {

	m_p_graphics = Graphics::GetInstance();
}


void EffectFile::InitShader(
	const std::string& shader_name,
	const std::string& tech_name
) {

	// それぞれの名前を代入
	this->shader_name = shader_name;
	this->tech_name = tech_name;

	// それぞれの初期化
	InitVertexDecl();

	if (CreateShader() == false) {
		return;
	}
}


void EffectFile::InitVertexDecl() {


	// 頂点要素定義
	D3DVERTEXELEMENT9 elem[] =
	{
		// 位置
		{
			// ストリーム番号
			0,
			// オフセット 頂点数 * サイズ
			0,
			// 変数の型
			D3DDECLTYPE_FLOAT3,
			// ポリゴン分割
			D3DDECLMETHOD_DEFAULT,
			// セマンティクス
			D3DDECLUSAGE_POSITION,
			0
		},

		// 法線
		{
			// ストリーム番号
			0,
			// float 4バイト * 3 = 12
			12,
			// 変数の型
			D3DDECLTYPE_FLOAT3,
			// ポリゴン分割
			D3DDECLMETHOD_DEFAULT,
			// セマンティクス
			D3DDECLUSAGE_NORMAL,
			0
		},

		// カラー
		{
			// ストリーム番号
			0,
			// float 4バイト * 9
			24,
			// カラー
			D3DDECLTYPE_D3DCOLOR,
			// ポリゴン分割
			D3DDECLMETHOD_DEFAULT,
			// セマンティクス
			D3DDECLUSAGE_COLOR,
			0
		},

		// uv
		{
			// ストリーム番号
			0,
			// float 4バイト * 9 + unsigned long 4バイト(カラー)
			28,
			// 変数の型
			D3DDECLTYPE_FLOAT2,
			// ポリゴン分割
			D3DDECLMETHOD_DEFAULT,
			// セマンティクス
			D3DDECLUSAGE_TEXCOORD,
			0
		},

		// 重み
		{
			// ストリーム番号
			0,
			// float 4バイト * 9 + 
			// unsigned long 4バイト(カラー) + 
			// float 4バイト * 2
			36,
			// 変数の型
			D3DDECLTYPE_FLOAT4,
			// ポリゴン分割
			D3DDECLMETHOD_DEFAULT,
			// セマンティクス
			D3DDECLUSAGE_BLENDWEIGHT,
			0
		},

		// 重みインデックス
		{
			// ストリーム番号
			0,
			// float 4バイト * 9 + 
			// unsigned long 4バイト(カラー) +
			// float 4バイト * 2 +
			// float 4バイト
			40,
			// 変数の型
			D3DDECLTYPE_UBYTE4,
			// ポリゴン分割
			D3DDECLMETHOD_DEFAULT,
			// セマンティクス
			D3DDECLUSAGE_BLENDINDICES,
			0
		},

		// 終了
		D3DDECL_END()
	};

	// 頂点データをシェーダー用のデータに変換
	HRESULT hr = m_p_graphics->GetInstance()->
		GetLpDirect3DDevice9()->
		CreateVertexDeclaration(
			elem,
			&m_p_decl
		);

	// 読み込めてなかったら
	if (hr != S_OK){
		return;
	}
}


void EffectFile::SetTexture(
	const std::string& tex_name,
	IDirect3DTexture9* p_tex
) {

	HRESULT hr = m_p_effect->SetTexture(tex_name.c_str(), p_tex);

	if (hr != S_OK) {
		// 失敗
		hr = S_FALSE;
	}
}


void EffectFile::ShaderUpdateMatrixArray(
	const D3DXHANDLE& handle,
	const D3DXMATRIX* update_mat,
	const int &count
) {

	// 行列配列をセット
	m_p_effect->SetMatrixArray(handle, update_mat, count);
}


bool EffectFile::CreateShader() {

	LPD3DXBUFFER error = nullptr;

	// シェーダーの作成
	D3DXCreateEffectFromFile(
		// デバイス
		m_p_graphics->GetInstance()->GetLpDirect3DDevice9(),
		// シェーダーファイル名
		TEXT(shader_name.c_str()),
		0,
		0,
		// シェーダーコンパイル時のオプション
		D3DXSHADER_DEBUG,
		0,
		// 作成されたシェーダーデータを格納するeffect
		&m_p_effect,
		// エラーデータ
		&error
	);

	if (m_p_effect == nullptr) {
		return false;
	}

	// グローバル変数のハンドル取得

	// アニメーション用
	m_bone_mat_h =
		m_p_effect->GetParameterByName(NULL, "g_bone_mat");
	//m_anim_mat_h =
	//	m_p_effect->GetParameterByName(NULL, "g_anim_mat");

	// 3Dパラメータ
	m_view_mat_h =
		m_p_effect->GetParameterByName(NULL, "g_view_mat");
	m_proj_mat_h =
		m_p_effect->GetParameterByName(NULL, "g_proj_mat");

	// ワールド
	m_world_mat_h =
		m_p_effect->GetParameterByName(NULL, "g_world_mat");

	// テクスチャ
	m_texture_h =
		m_p_effect->GetParameterByName(NULL, "g_tex");


	// テクニック取得
	m_tech_h = m_p_effect->GetTechniqueByName(tech_name.c_str());

	return true;
}


void EffectFile::UpdateShader(
	const D3DXMATRIX& world_mat,
	const D3DXMATRIX& bone_mat
) {

	// 頂点フォーマットの設定
	ShaderUpdateDecl();

	D3DXMATRIX view, proj;

	// ビュー行列
	m_p_graphics->
		GetInstance()->
		GetLpDirect3DDevice9()
		->GetTransform(D3DTS_VIEW, &view);

	// 射影行列 
	m_p_graphics->
		GetInstance()->
		GetLpDirect3DDevice9()
		->GetTransform(D3DTS_PROJECTION, &proj);

	// ビュー行列セット
	ShaderUpdateMatrix(m_view_mat_h, view);
	// プロジェクション行列セット
	ShaderUpdateMatrix(m_proj_mat_h, proj);
	// ボーン姿勢行列セット
	ShaderUpdateMatrix(m_bone_mat_h, bone_mat);
	// ワールド行列セット
	ShaderUpdateMatrix(m_world_mat_h, world_mat);
	//// アニメーション行列セット
	//ShaderUpdateMatrix(m_anim_mat_h, anim_mat);

	// テクニックをセット
	m_p_effect->SetTechnique(m_tech_h);

	// シェーダー更新
	CommitShader();
}


void EffectFile::ShaderUpdateDecl() {

	m_p_graphics->GetInstance()->
		GetLpDirect3DDevice9()->
		SetVertexDeclaration(
			m_p_decl
		);
}


void EffectFile::ShaderUpdateTech(
	const D3DXHANDLE& handle
) {
	m_p_effect->SetTechnique(m_tech_h);
}


void EffectFile::ShaderUpdateMatrix(
	const D3DXHANDLE& handle,
	const D3DXMATRIX& update_mat
) {

	m_p_effect->SetMatrix(handle, &update_mat);
}


void EffectFile::ShaderUpdateVector(
	const D3DXHANDLE& handle,
	D3DXVECTOR4& vec4
) {
	m_p_effect->SetVector(handle, &vec4);
}


void EffectFile::CommitShader() {
	m_p_effect->CommitChanges();
}


void EffectFile::ShaderBegin() {

	// シェーダー起動
	m_p_effect->Begin(
		// 起動したシェーダーのパスの数
		0,
		// デバイスステートの保存設定フラグ 
		0
	);

	// パス起動
	m_p_effect->BeginPass(
		// 起動したシェーダーのパスの数
		0
	);
}


void EffectFile::ShaderEnd() {

	// パス終了
	if (m_p_effect->EndPass() != S_OK) {

	}


	// シェーダー終了
	if (m_p_effect->End() != S_OK) {

	}
}

