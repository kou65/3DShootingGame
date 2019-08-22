#pragma once
#include"../D3D9/D3D9.h"
#include<vector>


// 3Dテクスチャデータ情報


struct TextureData3D{

	// テクスチャで使用するポインタ
	//std::vector<LPDIRECT3DTEXTURE9>texture_list;
	LPDIRECT3DTEXTURE9 texture_list[10];
};