#pragma once
#include"../ShaderBase/EffectFileBase.h"
#include"../ZTexture/ZTexture.h"



class DepthShadowEffectFile{
public:

	DepthShadowEffectFile();

	bool Init(const char*file_name);

	bool SetShandowMap(IDirect3DTexture9*shandow_map);

	void SetWorldMatrix(D3DXMATRIX*mat);

	void SetCameraViewMatrix(D3DXMATRIX*mat);

	void SetCameraProjMatrix(D3DXMATRIX*mat);

	void SetLightViewMatrix(D3DXMATRIX*mat);

	void SetLightProjMatrix(D3DXMATRIX*mat);


	// 描画の開始を宣言する
	HRESULT Begin();

	// 描画の終了を宣言する
	HRESULT End();

	// パスの開始を宣言する
	HRESULT BeginPass();

	// パスの終了を宣言する
	HRESULT EndPass();

	// 登録されているパラメータ情報をエフェクトにセット
	bool SetParamToEffect();

private:

	Graphics*m_p_graphics;
	IDirect3DTexture9*m_p_shadow_map_tex;
	ID3DXEffect *m_p_effect;

	D3DXMATRIX m_mat_world;

	D3DXMATRIX m_mat_camera_view;
	D3DXMATRIX m_mat_camera_proj;
	D3DXMATRIX m_mat_light_view;
	D3DXMATRIX m_mat_light_proj;

	D3DXHANDLE m_h_mat_world;
	D3DXHANDLE m_h_camera_view_mat;
	D3DXHANDLE m_h_camera_proj_mat;
	D3DXHANDLE m_h_light_view_mat;
	D3DXHANDLE m_h_light_proj_mat;
	D3DXHANDLE m_h_technique;
	D3DXHANDLE m_h_shadow_map_tex;

};