#pragma once
#include"../../Graphics/Graphics.h"



/*

3Dオブジェクトで扱うCustomVertex

*/

enum VertexType {
	VERTEX,
	UV,
	NORMAL,
	TOTAL_VERTEX_TYPE,
};


#define FVF_CUSTOM (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)


struct Object3DCustomVertex {

	Object3DCustomVertex() {

		vertex.x = vertex.y = vertex.z = 0.f;
		normal.x = normal.y = normal.z = 0.f;
		uv.x = uv.y = uv.z = 0.f;
	}

	// 頂点
	D3DXVECTOR3 vertex;

	// 法線
	D3DXVECTOR3 normal;
	
	// テクスチャ座標
	D3DXVECTOR3 uv;
};