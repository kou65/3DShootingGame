
#ifndef DEFAULT_LIGHT_H
#define DEFAULT_LIGHT_H
#include"../../Graphics/Graphics.h"


/**
* @brief DirectX標準のライトクラス
*/
class DefaultLight {
public:

	
	/**
	* @brief コンストラクタ
	* @param[in] graphics グラフィックスクラスポインタ
	* @param[in] set_light D3DLIGHT9構造体
	*/
	DefaultLight(
		Graphics*graphics,
		const D3DLIGHT9 &set_light
	);


	/**
	* @brief デフォルトライト用コンストラクタ
	*/
	DefaultLight(
		Graphics*graphics
	);


	/**
	* @brief ライトを光らせる
	*/
	bool On();

private:


	/**
	* @brief 標準ライト設定
	*/
	void DefaultLightMode();


private:

	//! D3Dライト
	D3DLIGHT9 m_light;

	//! DirectGraphics
	Graphics *graphics;
};
#endif