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


#define FVF_CUSTOM (D3DFVF_XYZ | /*D3DFVF_DIFFUSE |*/ D3DFVF_TEX1)


struct Object3DCustomVertex {

	Object3DCustomVertex() {

		position.x = position.y = position.z = 0.f;
		//normal.x = normal.y = normal.z = -1.f;
		//uv.x = uv.y = 0.f;
		//color = 0xffffffff;
	}

	// 頂点
	D3DXVECTOR3 position;


	//DWORD color;

	// テクスチャ座標
	//D3DXVECTOR2 uv;

	// 法線
	//D3DXVECTOR3 normal;
};