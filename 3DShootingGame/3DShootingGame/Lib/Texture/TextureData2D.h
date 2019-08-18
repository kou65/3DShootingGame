#pragma once
#include"TextureDataBase.h"



// 2Dテクスチャデータ



struct TextureData2D : public TextureDataBase {

	TextureData2D() {
		width_size = 0.f;
		height_size = 0.f;
	}

	// UV座標
	D3DXVECTOR2 uv;

	// 横サイズ
	float width_size;

	// 縦サイズ
	float height_size;
};