#pragma once
#include"../Graphics/Graphics.h"



/**
* @brief バックバッファを使用するサーフェイスクラス
*/
class Surface{
public:


	/**
	* @brief コンストラクタ
	*/
	Surface();

	/* 以下拡張演算子 */

	operator IDirect3DSurface9*() const {
		return mp_surface;
	}

	void operator =(IDirect3DSurface9*surface){
		mp_surface = surface;
	}

	/**
	* @brief 解放処理
	*/
	void Release();


	/**
	* @brief 作成
	* @param[in] width 解像度サイズ
	* @param[in] height 解像度サイズ
	* @param[in] format 書き込みフォーマット
	*/
	void CraeteDepthSurface(
		const int &width = 1920.f,
		const int &height = 1080.f,
		const D3DFORMAT&format = D3DFMT_D16
	);

	/**
	* @brief テクスチャを元にサーフェスを作成
	*/
	void CreateTextureSurface(
		LPDIRECT3DTEXTURE9& p_tex
	);


private:


	//! レンダリングを記録するサーフェイス
	LPDIRECT3DSURFACE9 mp_surface;

	//! デバイス
	IDirect3DDevice9*mp_device;
};