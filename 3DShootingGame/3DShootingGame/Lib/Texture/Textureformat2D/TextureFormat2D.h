#pragma once
#include"../DrawPolygon2D/DrawPolygon2D.h"
#include<Windows.h>
#include<string>


// テクスチャのプロパティを設定する構造体



struct TextureFormat2D {


	// コンストラクタ
	TextureFormat2D(
		float x,
		float y,
		const char * texture_name,
		float scale_width = 1.f,
		float scale_height = 1.f,
		float angle = 0.f,
		float ofset_x = 0.f,
		float ofset_y = 0.f,
		bool is_graph_uv_cut = false,
		int tu_cut_num = 0,
		int tv_cut_num = 0,
		int graph_num = 0,
		float tu = 0.f,
		float tv = 0.f,
		DrawPolygon2D::SamplerStateType state_type = DrawPolygon2D::CLAMP
	);


	// 取り出すテクスチャ名(登録時の名前)
	const char*texture_name;

	// x座標
	float x;

	// y座標
	float y;

	// 横の拡縮
	float scale_width;

	// 縦の拡縮
	float scale_height;

	// 角度
	float angle;

	// オフセットx(描画頂点をずらす)
	float ofset_x;

	// オフセットy
	float ofset_y;

	// 画像をカットするかどうか
	bool is_graph_uv_cut;

	// u軸のカット数
	int tu_cut_num;

	// v軸のカット数
	int tv_cut_num;

	// カットした画像のどこを使うか
	int graph_num;

	// テクスチャ座標のu軸をずらす
	float tu;

	// テクスチャ座標のv軸をずらす
	float tv;

	// サンプラーステートの状態
	DrawPolygon2D::SamplerStateType type;
};
