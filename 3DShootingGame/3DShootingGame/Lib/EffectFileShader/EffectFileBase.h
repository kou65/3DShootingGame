#pragma once
#include"../Graphics/Graphics.h"
#include<string>



class EffectFileBase {
public:

	// コンストラクタ
	EffectFileBase();

	// 初期化
	virtual bool Init();

	// 更新
	virtual void Update();

	// テクスチャをセット
	void SetTexture(
		IDirect3DTexture9*p_tex
	);

	// シェーダー名代入
	void SetShaderName(const std::string&shader_name);

	// テクニック名代入
	void SetTechnique(const std::string&tech_name);

	// ワールド行列をセット
	void SetWorldMatrix(const D3DXMATRIX &world_mat);

	// デフォルトのカメラビュー射影変換を行う
	void SetDefauleCamera();
	
	// シェーダーの開始
	void ShaderBegin(
		UINT &total_pass_num,
		const DWORD &device_state_num);

	// パス開始
	void BeginPass(const int &pass_num = 0);

	// パス終了
	void EndPass();

	// シェーダーの終了
	void ShaderEnd();

	// パラメータの更新
	void Set3DParam();

protected:

	// シェーダーの初期化
	void InitBaseShader(
		const std::string& shader_name,
		const std::string& tech_name
	);


	// シェーダー作成
	bool CreateBaseShader(
		const char*view_mat = "g_view_mat",
		const char*proj_mat = "g_proj_mat",
		const char*world_mat = "g_world_mat",
		const char*tex = "g_tex"
	);

	// シェーダーのパラメータをセットする
	void SetBaseParam();

	// 行列配列
	void ShaderUpdateMatrixArray(
		const D3DXHANDLE& handle,
		const D3DXMATRIX* update_mat,
		const int& count
	);

	// 頂点フォーマットの設定
	void SetDeclaration();


	// シェーダーの更新
	void CommitShader();

protected:


	// エフェクトシェーダー
	ID3DXEffect* m_p_effect;
	// デクル
	LPDIRECT3DVERTEXDECLARATION9 m_p_decl;


	// テクニックハンドル
	D3DXHANDLE m_tech_h;
	// ビュー行列ハンドル
	D3DXHANDLE m_camera_view_mat_h;
	// 射影行列ハンドル
	D3DXHANDLE m_camera_proj_mat_h;
	// ワールド行列ハンドル
	D3DXHANDLE m_world_mat_h;
	// テクスチャハンドル
	D3DXHANDLE m_texture_h;

	// ワールド行列
	D3DXMATRIX m_world_mat;
	// カメラビュー行列
	D3DXMATRIX m_camera_view_mat;
	// カメラ射影変換行列
	D3DXMATRIX m_camera_proj_mat;

	// テクニック名
	std::string m_tech_name;
	// シェーダー名
	std::string m_shader_name;
	// グラフィックス
	Graphics* m_p_graphics;

};