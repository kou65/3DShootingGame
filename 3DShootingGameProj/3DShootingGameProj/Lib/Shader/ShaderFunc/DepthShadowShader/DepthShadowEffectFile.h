#pragma once
#include"../ShaderBase/EffectFileBase.h"
#include"../ZTexture/ZTexture.h"
#include"../LightData/LightData.h"



class DepthShadowShader : public StandardTSShader{
public:
	
	// コンストラクタ
	DepthShadowShader(
		const VertexDecl::Type &type = VertexDecl::Type::OBJ
	);

	// 初期化
	void Init();

	// 更新
	void Update()override;

	// 影テクスチャ
	bool SetShandowMap(IDirect3DTexture9*shandow_map);

	// テクスチャ
	bool SetTexture(IDirect3DTexture9*p_tex);

	// ライトビュー行列
	void SetLightViewMatrix(const D3DXMATRIX&mat);

	// ライト射影行列
	void SetLightProjMatrix(const D3DXMATRIX&mat);

	// ライトデータ挿入
	void SetLightData(const LightData&data);

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

	//! グラフィックスポインタ
	Graphics*mp_graphics;

	//! シャドウマップ
	IDirect3DTexture9*mp_shadow_map;

	//! ライトビュー
	D3DXMATRIX m_mat_light_view;

	//! ライト射影
	D3DXMATRIX m_mat_light_proj;

	// ライト系
	D3DXVECTOR4 m_light_dir;
	float m_specular;
	float m_specular_power;
	LightData m_light_data;

	//! 各ハンドル
	D3DXHANDLE m_h_light_view;
	D3DXHANDLE m_h_light_proj;
	D3DXHANDLE m_h_shadow_map;
	D3DXHANDLE m_h_tex;

	// ライトハンドル
	D3DXHANDLE m_h_light_dir;
	D3DXHANDLE m_h_specular;
	D3DXHANDLE m_h_specular_power;
	D3DXHANDLE m_h_ambient;
	D3DXHANDLE m_h_eye_dir;
	D3DXHANDLE m_h_eye_pos;

};