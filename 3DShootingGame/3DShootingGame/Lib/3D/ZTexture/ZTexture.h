#pragma once
#include"../../Graphics/Graphics.h"


class ZTexture {
public:

	ZTexture();

	void Create(const char*effect_file_name);


	bool GetSurfaceWH(IDirect3DSurface9* pSurf, UINT& uiWidth, UINT& uiHeight);

	void Begin();

	void BeginPass();

	void End() {};

private:

	// z値テクスチャ
	IDirect3DTexture9* m_p_tex;

	// z値テクスチャサーフェイス
	IDirect3DSurface9* m_p_tex_suf;

	// 深度バッファ
	IDirect3DSurface9* m_p_depth_buffer;

	// デバイスバックバッファ
	IDirect3DSurface9* m_p_device_buffer;

	// デバイス深度バッファ
	IDirect3DSurface9* m_p_device_depth;

	// zプロットエフェクト
	ID3DXEffect* m_p_effect;

	// ワールド
	D3DXMATRIX m_mat_world;

	// ビュー
	D3DXMATRIX m_mat_view;

	// 射影変換
	D3DXMATRIX m_mat_proj;

	// ワールド変換行列ハンドル
	D3DXHANDLE m_world_mat_handle;

	// ビュー変換行列ハンドル
	D3DXHANDLE m_view_mat_handle;

	// 射影変換行列ハンドル
	D3DXHANDLE m_proj_mat_handle;

	// テクニックへのハンドル
	D3DXHANDLE m_technique_handle;

	// デバイス
	IDirect3DDevice9 *m_p_device;
};