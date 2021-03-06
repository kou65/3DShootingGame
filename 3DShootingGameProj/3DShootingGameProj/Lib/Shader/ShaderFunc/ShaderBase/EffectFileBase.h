﻿#pragma once
#include"../../../Graphics/Graphics.h"
#include"../Declation/VertexDeclation.h"
#include<string>



/**
* @brief シェーダー基底クラス
*/
class ShaderBase {
public:

	// コンストラクタ
	ShaderBase();

	virtual ~ShaderBase() {}

	// 更新
	virtual void Update();

	// 描画関係
	
	// シェーダーの開始
	virtual void Begin(
		UINT &total_pass_num,
		const DWORD &device_state_num = 0
	);


	// シェーダーの終了
	virtual void End();

	// パス開始
	virtual void BeginPass(const int &out_pass_num = 0);

	// パス終了
	void EndPass();

	// バーテックスデクルのリセット
	void ResetDecl();

	// 解放処理
	void Release();


protected:

	// 継承先で使う用

	// シェーダー作成(任意のタイミングで生成してください)
	bool CreateEffectFile(
		const std::string&shader_file_path_name,
		const std::string&tech_name,
		const VertexDecl::Type&decl_type
	);

	// デクル更新
	void UpdateDecl();

	// シェーダーの更新
	void CommitShader();

protected:

	// 継承先で使わない

	// デクルを選択
	void SelectDeclCommit(const VertexDecl::Type&decl_type);

	// デフォルトのカメラビュー射影変換を行う
	void OutMatrixGrapicsCamera(D3DXMATRIX*view, D3DXMATRIX*proj);

	// テクニックデータをセット
	void CommitTechnique();

protected:

	// エフェクトシェーダー
	ID3DXEffect* mp_effect;

	// デクル(必ず設定)
	VertexDecl m_decl;

	// テクニックハンドル
	D3DXHANDLE m_h_technique;

};