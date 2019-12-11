#pragma once
#include"../../Graphics/Graphics.h"




struct AnimationCustomVertex {

	AnimationCustomVertex() {
		vertex.x = vertex.y = vertex.z = 0.f;
		normal.x = normal.y = normal.z = 0.f;
		uv.x = uv.y = 0.f;
		color = 0xfffffff;
	}

	// 頂点データ
	D3DXVECTOR3 vertex;

	// 法線ベクトル
	D3DXVECTOR3 normal;

	// カラー
	DWORD color;

	// テクセル
	D3DXVECTOR2 uv;

	// スキンメッシュ用
	BYTE bone_index[4];
	float weight[4];
};
