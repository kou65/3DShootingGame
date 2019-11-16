#pragma once
#include"../../Graphics/Graphics.h"
#include<vector>


// 3Dテクスチャデータ情報


struct TextureMultipleData{

	// テクスチャで使用するポインタ
	std::vector<LPDIRECT3DTEXTURE9>texture_list;
};