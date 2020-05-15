#pragma once
#include"../ShaderBase/EffectFileBase.h"
#include"../ZTexture/ZTexture.h"



class DepthShadowShader : public StandardTSShader{
public:
	
	// コンストラクタ
	DepthShadowShader(
		const VertexDecl::Type &type = VertexDecl::Type::OBJ
	);

	// 初期化
	void Init()override;

	// 更新
	void Update()override;

	// テクスチャ
	bool SetShandowMap(IDirect3DTexture9*shandow_map);

	// ライトビュー行列
	void SetLightViewMatrix(const D3DXMATRIX&mat);

	// ライト射影行列
	void SetLightProjMatrix(const D3DXMATRIX&mat);

	// デバイスのビューと射影情報を取ってくる
	void CommitDeviceViewProj();

	// 描画の開始を宣言する
	void Begin(
		UINT &total_pass_num,
		const DWORD &device_state_num
	)override;

	// 描画の終了を宣言する
	void End()override;


private:

	// 登録されているパラメータ情報をエフェクトにセット
	bool SetParamToEffect();

private:

	Graphics*m_p_graphics;
	IDirect3DTexture9*m_p_shadow_map_tex;

	D3DXMATRIX m_mat_light_view;
	D3DXMATRIX m_mat_light_proj;

	D3DXHANDLE m_h_light_view;
	D3DXHANDLE m_h_light_proj;
	D3DXHANDLE m_h_shadow_map_tex;

};