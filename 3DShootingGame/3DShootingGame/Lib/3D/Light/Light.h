#pragma once
#include"../../Graphics/Graphics.h"



class DefaultLight {
public:

	// ライト変更用コンストラクタ
	DefaultLight(
		Graphics*graphics,
		const D3DLIGHT9 &set_light
	);

	// デフォルトライト用コンストラクタ
	DefaultLight(
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