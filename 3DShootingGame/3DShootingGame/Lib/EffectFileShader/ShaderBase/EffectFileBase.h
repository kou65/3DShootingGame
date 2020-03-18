#pragma once
#include"../../Graphics/Graphics.h"
#include"../Declation/VertexDeclation.h"
#include<string>



class EffectFileBase {
public:

	// コンストラクタ
	EffectFileBase();

	// 初期化
	virtual void Init();

	// 更新
	virtual void Update();


	// シェーダー名代入
	void SetShaderName(const std::string&shader_name);

	// テクニック名代入
	void SetTechnique(const std::string&tech_name);

public:

	// 描画関係
	
	// シェーダーの開始
	void ShaderBegin(
		UINT &total_pass_num,
		const DWORD &device_state_num);

	// パス開始
	void BeginPass(const int &out_pass_num = 0);

	// パス終了
	void EndPass();

	// シェーダーの終了
	void ShaderEnd();

protected:

	// シェーダー作成(任意のタイミングで生成してください)
	bool CreateEffectFile();

	// デフォルトのカメラビュー射影変換を行う
	void OutDefauleCamera(D3DXMATRIX*view, D3DXMATRIX*proj);

	// シェーダーの更新
	void CommitShader();

protected:


	// エフェクトシェーダー
	ID3DXEffect* m_p_effect;
	// デクル
	VertexDecl m_p_decl;
	// テクニックハンドル
	D3DXHANDLE m_tech_h;
	// テクニック名
	std::string m_tech_name;
	// シェーダー名
	std::string m_shader_name;
	// グラフィックス
	Graphics* m_p_graphics;

};