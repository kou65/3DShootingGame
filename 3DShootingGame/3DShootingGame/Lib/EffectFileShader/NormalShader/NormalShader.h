#pragma once
#include"../ShaderBase/EffectFileBase.h"



class NormalShader : public EffectFileBase {
public:

	NormalShader() {

		D3DXMatrixIdentity(&m_world_mat);
	}

	void Init()override;
	void Update()override;

	void InitHandle();
	void SetColor(const D3DXVECTOR4 &color);
	void SetTexture(IDirect3DTexture9* p_tex);
	void SetWorldMatrix(D3DXMATRIX*world_mat);

private:

	void UpdateParam();

private:

	// ビュー行列ハンドル
	D3DXHANDLE m_camera_view_mat_h;
	// 射影行列ハンドル
	D3DXHANDLE m_camera_proj_mat_h;
	// ワールド行列ハンドル
	D3DXHANDLE m_world_mat_h;
	// テクスチャハンドル
	D3DXHANDLE m_texture_h;
	// カラーハンドル
	D3DXHANDLE m_color_h;


	// ワールド行列
	D3DXMATRIX m_world_mat;
	// カメラビュー行列
	D3DXMATRIX m_camera_view_mat;
	// カメラ射影変換行列
	D3DXMATRIX m_camera_proj_mat;
	// カラー値
	D3DXVECTOR4 m_color;
	// テクスチャポインタ
	IDirect3DTexture9*m_p_tex;

};