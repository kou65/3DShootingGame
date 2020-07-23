#pragma once
#include"../../../Graphics/Graphics.h"
#include"../StandardTransformShader/StandardTransformShader.h"
#include"../../MultRenderTarget/MultRenderTarget.h"

// Z値を自分で計算するZテクスチャを作成するクラス


/**
* @brief zテクスチャ
*/
class ZTexture : public StandardTSShader{
public:

	// コンストラクタ
	ZTexture(
		const VertexDecl::Type &type = VertexDecl::Type::OBJ
	);

	~ZTexture();

	// 初期化
	void Init();

	// 更新
	void Update()override;

	void Begin(
		UINT &total_pass_num,
		const DWORD &device_state_num)override;

	void End()override;

	// 初期化
	bool InitZTexture();

	// テクスチャ解像度サイズ
	void SetTextureSize(
		UINT z_tex_width,
		UINT z_tex_height
	);

	// テクスチャのポインタを返す
	IDirect3DTexture9* GetZTexture();

private:


	void InitHandle();

	// テクスチャ作成
	bool CreateTexture(
		UINT z_tex_width,
		UINT z_tex_height,
		D3DFORMAT z_tex_format
	);

	// サーフェイスサイズ取得
	bool GetSurfaceWH(IDirect3DSurface9* pSurf, UINT& uiWidth, UINT& uiHeight);

private:

	// 解像度
	UINT m_z_tex_width;
	UINT m_z_tex_height;

	// テクスチャフォーマット
	D3DFORMAT m_z_tex_format;

	// z値テクスチャ
	IDirect3DTexture9* mp_tex;

	// z値テクスチャサーフェイス
	IDirect3DSurface9* mp_tex_suf;

	// 深度バッファ
	IDirect3DSurface9* mp_depth_buffer;

	// デバイスバックバッファ
	IDirect3DSurface9* mp_device_buffer;

	// デバイス深度バッファ
	IDirect3DSurface9* mp_depth;

	// スタートパス数
	int m_start_pass_num;

	// デバイス
	IDirect3DDevice9 *mp_device;

};