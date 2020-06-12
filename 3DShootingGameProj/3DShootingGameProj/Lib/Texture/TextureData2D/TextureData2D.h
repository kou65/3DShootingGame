#pragma once
#include"../../Graphics/Graphics.h"


// 2Dテクスチャデータ



struct TextureData{

	TextureData() {
		width_size = 0.f;
		height_size = 0.f;
		uv.x = uv.y = 0.f;
		p_texture_buffer = nullptr;
	}


	// 自動検出してゲッターを行う
	operator LPDIRECT3DTEXTURE9() const {
		return p_texture_buffer;
	}

	// ロード時に読み込む用のUV座標
	D3DXVECTOR2 uv;

	// 横サイズ
	float width_size;

	// 縦サイズ
	float height_size;

	// テクスチャで使用するポインタ
	LPDIRECT3DTEXTURE9 p_texture_buffer;
};