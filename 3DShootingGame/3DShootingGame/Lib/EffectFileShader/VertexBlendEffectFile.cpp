#include"VertexBlendEffectFile.h"



VertexBlendEffectFile::VertexBlendEffectFile()
	: EffectFileBase(){

	//D3DXMatrixIdentity(&m_bone_mat);
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

	return true;
}


void VertexBlendEffectFile::Update() {

	// ボーン姿勢行列セット
	ShaderUpdateMatrixArray(
		m_bone_mat_h,
		m_bone_mat_list,
		m_bone_count
	);

	// 頂点フォーマットの設定
	SetDeclaration();

	// シェーダーの更新
	SetBaseParam();

}


void VertexBlendEffectFile::SetBoneMatrix(
	D3DXMATRIX*bone_mat
) {
	m_bone_mat_list = bone_mat;
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

		// 重み
		{
			// ストリーム番号
			0,
			// float 4バイト * 9 + 
			// unsigned long 4バイト(カラー) + 
			// float 4バイト * 2
			44,
			// 変数の型
			D3DDECLTYPE_FLOAT4,
			// ポリゴン分割
			D3DDECLMETHOD_DEFAULT,
			// セマンティクス
			D3DDECLUSAGE_BLENDWEIGHT,
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
