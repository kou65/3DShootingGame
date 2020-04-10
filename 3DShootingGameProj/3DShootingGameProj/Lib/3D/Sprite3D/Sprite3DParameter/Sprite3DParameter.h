#pragma once
#include<string>
#include"../../../Graphics/Graphics.h"


// テクスチャ3D書式


// ポリゴンの向き
enum PolygonDirection {
	FRONT,
	BACK,
	RIGHT,
	LEFT,
	FLOOR
};


struct Sprite3DParameter{


	Sprite3DParameter(
		float pos_x,
		float pos_y,
		float pos_z,
		const char* texture_name,
		PolygonDirection dir = FRONT,
		float scale_width = 1.f,
		float scale_height = 1.f,
		bool is_back_cull_mode = true,
		float ofset_x = 0.f,
		float ofset_y = 0.f,
		bool is_graph_uv_cut = false,
		int tu_cut_num = 0,
		int tv_cut_num = 0,
		int graph_num = 0,
		float tu = 0.f,
		float tv = 0.f,
		D3DXCOLOR color = 0xfffffff
	);

	// ポリゴン位置
	D3DXVECTOR3 pos;

	// テクスチャ登録時の名前
	const char* texture_name;

	// 横の拡縮
	float scale_width;

	// 縦の拡縮
	float scale_height;

	// ポリゴンの向き
	PolygonDirection polygon_dir;

	// 背面カリングをおこなうかどうか
	bool is_back_cull_mode;

	// オフセット(ポリゴン頂点修正値)
	D3DXVECTOR2 ofset;

	// 画像をカットするかどうか
	bool is_graph_uv_cut;

	// 横のUVカット数
	int tu_cut_num;

	// 縦のUVカット数
	int tv_cut_num;

	// 画像数
	int graph_num;

	// テクスチャ座標(U値)
	float tu;
	
	// テクスチャ座標(V値)
	float tv;

	// 頂点カラー
	D3DXCOLOR color;
};