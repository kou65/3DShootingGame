#pragma once
#include"../../Graphics/Graphics.h"



struct AnimationCustomVertex {

	// コンストラクタあり
	AnimationCustomVertex();

	// 頂点データ
	D3DXVECTOR3 vertex;

	// 法線ベクトル
	D3DXVECTOR3 normal;

	// カラー
	DWORD color;

	// テクセル
	D3DXVECTOR2 uv;

	// スキンメッシュ用重み(この頂点で影響する重みを全て持たせる)
	float weight[4];

	// スキンメッシュ用影響するボーン番号(頂点)
	int bone_index[4];
};
