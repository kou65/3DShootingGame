#pragma once
#include"../../../Graphics/Graphics.h"



struct IndexCustomVertex {

	// 頂点情報
	D3DXVECTOR3 vertex;

	// 法線情報
	D3DXVECTOR3 normal;

	// 頂点カラー
	DWORD color;

	// テクスチャ座標
	D3DXVECTOR2 uv;
};