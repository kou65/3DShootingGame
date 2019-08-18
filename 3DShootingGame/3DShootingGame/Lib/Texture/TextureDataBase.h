#pragma once

#include<d3dx9.h>

#pragma comment(lib,"d3dx9.lib")


// 基底テクスチャデータ


struct TextureDataBase {


	// 自動検出してゲッターを行う
	operator LPDIRECT3DTEXTURE9() const {
		return p_texture_buffer;
	}


	// 仮想デストラクタ
	virtual ~TextureDataBase() {};


	// テクスチャで使用するポインタ
	LPDIRECT3DTEXTURE9 p_texture_buffer;
};