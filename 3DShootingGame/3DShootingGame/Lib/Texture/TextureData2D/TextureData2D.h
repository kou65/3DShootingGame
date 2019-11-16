#pragma once
#include"../../Graphics/Graphics.h"


// 2Dテクスチャデータ



struct TextureData{

	TextureData() {
		width_size = 0.f;
		height_size = 0.f;
	}

	// UV座標
	D3DXVECTOR2 uv;

	// 横サイズ
	float width_size;

	// 縦サイズ
	float height_size;

	// 自動検出してゲッターを行う
	operator LPDIRECT3DTEXTURE9() const {
		return p_texture_buffer;
	}


	// 仮想デストラクタ
	virtual ~TextureData() {};


	// テクスチャで使用するポインタ
	LPDIRECT3DTEXTURE9 p_texture_buffer;
};