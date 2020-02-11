#pragma once
#include"../Graphics/Graphics.h"
#include"../EffectFileShader/EffectFileBase.h"


// Z値を自分で計算するZテクスチャを作成するクラス


class ZTexture{
public:

	ZTexture();

	bool Init(
		const char*effect_file_name,
		UINT z_tex_width,
		UINT z_tex_height,
		D3DFORMAT z_tex_format = D3DFMT_A8B8G8R8
	);

	bool GetSurfaceWH(IDirect3DSurface9* pSurf, UINT& uiWidth, UINT& uiHeight);

	void SetWorldMatrix(D3DXMATRIX*mat);

	void SetViewMatrix(D3DXMATRIX*mat);

	void SetProjMatrix(D3DXMATRIX*mat);

	bool SetParameter();

	void Begin();

	void BeginPass();

	void End();

	void EndPass();

	IDirect3DTexture9* GetZTexture();

private:

	// エフェクト
	ID3DXEffect*m_p_effect;

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

	// スタートパス数
	int m_start_pass_num;

	// ハンドル
	D3DXHANDLE m_h_world_mat;
	D3DXHANDLE m_h_view_mat;
	D3DXHANDLE m_h_proj_mat;
	D3DXHANDLE m_h_tech;

	// 行列
	D3DXMATRIX m_world_mat;
	D3DXMATRIX m_view_mat;
	D3DXMATRIX m_proj_mat;

	// デバイス
	IDirect3DDevice9 *m_p_device;
};