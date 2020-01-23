#pragma once
#include"../Graphics/Graphics.h"
#include<string>




class EffectFile {
public:

	// コンストラクタ
	EffectFile();

	// シェーダーの初期化
	void InitShader(
		const std::string& shader_name,
		const std::string& tech_name
	);

	// シェーダーの更新
	void UpdateShader(
		const D3DXMATRIX& world_mat,
		const D3DXMATRIX& bone_mat
	);

	void SetTexture(
		const std::string& tex_name,
		IDirect3DTexture9*p_tex
	);

	// シェーダーの開始
	void ShaderBegin();

	// シェーダーの終了
	void ShaderEnd();

private:

	// シェーダーの更新
	void CommitShader();

	// 頂点シェーダーの初期化
	void InitVertexDecl();

	// シェーダー作成
	bool CreateShader();

	// 頂点フォーマットの設定
	void ShaderUpdateDecl();

	// テクニックの更新
	void ShaderUpdateTech(
		const D3DXHANDLE& handle
	);

	// パラメーターの設定、更新
	void ShaderUpdateMatrix(
		const D3DXHANDLE& handle,
		const D3DXMATRIX& update_mat
	);

	// 行列配列
	void ShaderUpdateMatrixArray(
		const D3DXHANDLE& handle,
		const D3DXMATRIX* update_mat,
		const int& count
	);

	void ShaderUpdateVector(
		const D3DXHANDLE& handle,
		D3DXVECTOR4& vec4
	);

private:


	// エフェクトシェーダー
	ID3DXEffect* m_p_effect;
	// デクル
	LPDIRECT3DVERTEXDECLARATION9 m_p_decl;
	// テクニックハンドル
	D3DXHANDLE m_tech_h;
	// アニメーション行列ハンドル
	D3DXHANDLE m_anim_mat_h;
	// ボーン行列ハンドル
	D3DXHANDLE m_bone_mat_h;
	// ビュー行列ハンドル
	D3DXHANDLE m_view_mat_h;
	// 射影行列ハンドル
	D3DXHANDLE m_proj_mat_h;
	// ワールド行列ハンドル
	D3DXHANDLE m_world_mat_h;
	// テクスチャハンドル
	D3DXHANDLE m_texture_h;


	// テクニック名
	std::string tech_name;

	// シェーダー名
	std::string shader_name;

	// グラフィックス
	Graphics* m_p_graphics;
};