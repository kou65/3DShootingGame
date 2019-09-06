#pragma once
#include"../../Graphics/Graphics.h"
#include<vector>


// 3Dテクスチャデータ情報


struct TextureData3D{

	// テクスチャで使用するポインタ
	std::vector<LPDIRECT3DTEXTURE9>texture_list;
};