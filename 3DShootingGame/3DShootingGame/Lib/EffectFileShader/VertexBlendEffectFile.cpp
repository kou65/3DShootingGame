#include"VertexBlendEffectFile.h"



VertexBlendEffectFile::VertexBlendEffectFile()
	: EffectFileBase(){

	m_bone_count = 64;
}


bool VertexBlendEffectFile::Init(){

	// 通常のシェーダー初期化
	InitBaseShader(m_shader_name,m_tech_name);

	// デクラレーション初期化
	InitVertexDecl();

	// アニメーション用
	m_bone_mat_h = 
		m_p_effect->
		GetParameterByName(NULL, "g_bone_mat");

	// インデックス用
	m_total_index_h =
		m_p_effect->GetParameterByName(NULL, "g_max_blend_idx");

	return true;
}


void VertexBlendEffectFile::Update() {

	// 現在設定しているカメラを入れる
	SetDefauleCamera();

	// ボーン姿勢行列セット
	m_p_effect->SetMatrixArray(
		m_bone_mat_h,
		m_bone_mat_list,
		64
	);

	m_p_effect->SetInt(m_total_index_h,m_max_index);

	// 頂点フォーマットの設定
	SetDeclaration();

	// シェーダーの更新
	SetBaseParam();

}


void VertexBlendEffectFile::SetMaxIndex(const int&index) {

	m_max_index = index;
}


void VertexBlendEffectFile::SetBoneMatrix(
	D3DXMATRIX*bone_mat
) {
	m_bone_mat_list = bone_mat;
}

void VertexBlendEffectFile::SetBoneMatrixArray(
	D3DXHANDLE&handle,
	D3DXMATRIX*mat,
	const int &array_num
) {

	// 行列配列をセット
	HRESULT hr 
		= m_p_effect->SetMatrixPointerArray(
			handle,
			(const D3DXMATRIX**)&mat,
			array_num
		);

}


void VertexBlendEffectFile::SetBoneCount(const int&count) {

	m_bone_count = count;
}

void VertexBlendEffectFile::InitVertexDecl() {


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
			D3DDECLTYPE_FLOAT4,
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
			16,
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
			28,
			// カラー(UBYTE4)
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
			32,
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
			40,
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
			56,
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
		GetDevice()->
		CreateVertexDeclaration(
			elem,
			&m_p_decl
		);


	// 読み込めてなかったら
	if (hr != S_OK) {
		hr = S_OK;
		return;
	}
}
