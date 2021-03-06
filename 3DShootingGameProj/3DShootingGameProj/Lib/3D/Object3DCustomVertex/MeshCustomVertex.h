﻿

#ifndef MESH_CUSTOM_VERTEX
#define MESH_CUSTOM_VERTEX

#include"../../Graphics/Graphics.h"



/*

3Dオブジェクトで扱うCustomVertex

*/

/**
* @enum VertexType 頂点の種類
*/
enum VertexType {
	VERTEX,
	UV,
	NORMAL,
	TOTAL_VERTEX_TYPE,
};


/**
頂点データ
位置
法線
頂点色(ディフューズ色)
テクスチャ座標
*/
#define FVF_CUSTOM (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_DIFFUSE)


/**
* @brief メッシュ用頂点
*/
struct MeshCustomVertex {

	MeshCustomVertex() {

		position.x = position.y = position.z = 0.f;
		normal.x = normal.y = normal.z = 0.f;
		color = 0xffffff;
		uv.x = uv.y = 1.f;
	}

	// 頂点
	D3DXVECTOR3 position;
	// 法線
	D3DXVECTOR3 normal;
	// 頂点カラー
	DWORD color;
	// テクスチャ座標
	D3DXVECTOR2 uv;
};

#endif