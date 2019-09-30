#pragma once
#include"../../Graphics/Graphics.h"



class Light {
public:

	// ライト変更用コンストラクタ
	Light(
		Graphics*graphics,
		const D3DLIGHT9 &set_light
	);

	// デフォルトライト用コンストラクタ
	Light(
		Graphics*graphics
	);

	// ライトを光らせる
	bool On();

private:

	// 標準ライト設定
	void DefaultLightMode();

private:

	// D3Dライト
	D3DLIGHT9 m_light;

	// DirectGraphics
	Graphics *graphics;
};