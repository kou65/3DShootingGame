#pragma once
#include"../../Graphics/Graphics.h"



/**
* @brief マルチレンダリング用バックバッファ保存クラス
*/
class BackBuffer {
public:


	/**
	* @brief コンストラクタ
	*/
	BackBuffer();


	~BackBuffer();


	/**
	* @brief バックバッファ保存
	*/
	void SaveBuffer(
		const int &target_index = 0
	);


	/**
	* @brief バックバッファを現在のバッファに戻す
	*/
	void SetBuffer(
		const int&target_index = 0
	);

private:

	//! デバイス
	LPDIRECT3DDEVICE9 mp_device;

	//! デバイスのバッファ
	LPDIRECT3DSURFACE9 mp_device_buffer;

	//! デバイスの深度バッファ
	LPDIRECT3DSURFACE9 mp_device_depth_buffer;

	//! ビューポート
	D3DVIEWPORT9 m_view_port;
};