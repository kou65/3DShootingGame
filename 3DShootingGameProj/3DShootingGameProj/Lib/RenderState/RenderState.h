#ifndef RENDER_STATE_H
#define RENDER_STATE_H

#include"../Graphics/Graphics.h"


namespace RenderState {

	// SetRenderStateの設定を行う
	void AllOn();

	// α値が有効か
	void AlphaEnable(
		BOOL is_enable
	);

	// αテスト
	void AlphaTest(
		BOOL is_enable,
		int value = 0x000000);

	// αブレンド
	void AlphaBlendOn();

	// シェーダーモード
	void ShadeMode(const D3DSHADEMODE&mode = D3DSHADE_GOURAUD);

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

#endif 