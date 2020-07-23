#pragma once
#include"../../Graphics/Graphics.h"
#include"../../Vec2/Vec2.h"
#include"../RenderTargetData/RenderTargetData.h"



/**
* @brief マルチレンダリングターゲット用
*/
class RenderTarget {
public:


	/**
	* @brief コンストラクタ
	*/
	RenderTarget();

	~RenderTarget();


	/**
	* @brief 作成
	*/
	bool CreateSurface(
		RenderTargetData&data
	);

	/**
	* @brief 削除
	*/
	void DestorySurface();


	/**
	* @brief レンダーターゲットセット
	*/
	void SetRenderTarget(const int&target = 0);


	/**
	* @brief 深度値セット
	*/
	void SetDepth();

	void GetRenderTarget(
		LPDIRECT3DSURFACE9&suf,
		const int&target = 0
	);

	void GetDepth(LPDIRECT3DSURFACE9&suf);

	/**
	* @brief サーフェイスが使える状態かチェック
	*/
	bool CheckSurface();


	/**
	* @brief テクスチャ取得
	*/
	void GetTexture(LPDIRECT3DTEXTURE9&tex);

	LPDIRECT3DTEXTURE9 GetTexture();

	// ビューポートセット
	void SetViewPort();

private:

	bool CreateTexture();

	bool CreateDepth();

	bool CreateTexSurface();

	void CreateViewPort(
		Vec2 pos,
		DWORD width,
		DWORD height
	);

private:

	// テクスチャ保存用
	LPDIRECT3DTEXTURE9 mp_tex;

	LPDIRECT3DSURFACE9 mp_tex_surface;

	// 3D用深度サーフェイス
	LPDIRECT3DSURFACE9 mp_depth;

	// ビューポート
	D3DVIEWPORT9 m_view_port;

	// デバイス
	LPDIRECT3DDEVICE9 mp_device;

	DWORD m_width_size;
	DWORD m_height_size;
};