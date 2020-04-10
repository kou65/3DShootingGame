#pragma once
#include"../ShaderBase/EffectFileBase.h"
#include"../ZTexture/ZTexture.h"



class DepthShadowShader : public StandardTransformShader{
public:

	DepthShadowShader();

	// 初期化
	void Init()override;

	// 更新
	void Update()override;

	bool SetShandowMap(IDirect3DTexture9*shandow_map);

	void SetLightViewMatrix(const D3DXMATRIX&mat);

	void SetLightProjMatrix(const D3DXMATRIX&mat);


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

	D3DXHANDLE m_h_light_view_mat;
	D3DXHANDLE m_h_light_proj_mat;
	D3DXHANDLE m_h_shadow_map_tex;

};