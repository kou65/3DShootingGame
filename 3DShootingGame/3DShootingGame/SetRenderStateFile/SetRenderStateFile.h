#pragma once
#include"../Lib/D3D9/D3D9.h"


namespace SetRenderStateFile {

	// SetRenderStateの設定を行う
	void Init();

	// α値が有効か
	void AlphaEnable(
		BOOL is_enable
	);

	// αテスト
	void AlphaTest(
		BOOL is_enable,
		int value = 0x000000);

	// αブレンド
	void AlphaBlend();

	// シェーダーモード
	void ShadeMode();

	// Zバッファ
	void ZBufferMode(
		BOOL is_enable
	);

	// Lighting
	void LightMode(
		BOOL is_enable
	);

	// カリングモード
	void CullMode(
		BOOL is_enable
	);
}