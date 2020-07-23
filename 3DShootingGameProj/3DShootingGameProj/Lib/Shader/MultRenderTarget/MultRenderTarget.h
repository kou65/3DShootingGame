#pragma once
#include"../RenderTarget/RenderTarget.h"
#include"../BackBuffer/BackBuffer.h"
#include"../RenderTargetData/RenderTargetData.h"
#include<vector>



/**
* @brief マルチレンダリング用
*/
class MultRenderTarget {
public:


	/**
	* @brief コンストラクタ
	*/
	MultRenderTarget(
		const int&render_index = 1
	);


	/**
	* @brief バックバッファを受け取る
	*/
	void SetBackBuffer();


	/**
	* @brief マルチレンダリング描画
	*/
	void SetRender(
		RenderTarget&rt
	);


	/**
	* @brief マルチレンダリング終了
	*/
	void GetBackBuffer();


	/**
	* @brief テクスチャ取得
	*/
	void SetTexture(RenderTarget&rt);


	/**
	* @brief 現在のデバイスにテクスチャに保存
	*/
	void SetDeviceTexture(LPDIRECT3DTEXTURE9&tex);


private:

	//! バックバッファ
	BackBuffer mp_back_buf;

	//! 現在合成中のテクスチャ
	LPDIRECT3DTEXTURE9 mp_tex;
};