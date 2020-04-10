#pragma once
#include"../../Graphics/Graphics.h"
#include<Windows.h>
#include<string>
#include"../../Uv/AnimationParameter/AnimationParameter.h"
#include"../../Vec2/Vec2.h"


// テクスチャのプロパティを設定する構造体



struct Sprite2DParameter {


	// コンストラクタ
	Sprite2DParameter(
		float x = 0.f,
		float y = 0.f,
		const char * texture_name = NULL,
		float scale_width = 1.f,
		float scale_height = 1.f,
		float angle = 0.f,
		float ofset_x = 0.f,
		float ofset_y = 0.f,
		float tu = 0.f,
		float tv = 0.f,
		DWORD color = D3DCOLOR(0xffffff),
		Graphics::SamplerStateType state_type = Graphics::MIRROR
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

	// 横の画像の大きさ
	float width_size;

	// 縦の画像の大きさ
	float height_size;

	// 角度
	float angle;

	// オフセットx(描画頂点をずらす)
	float ofset_x;

	// オフセットy
	float ofset_y;

	// アニメーション用のパラメータ
	AnimationParamter animation_param;

	bool is_uv_slider;

	// テクスチャ座標のu軸をずらす
	float tu;

	// テクスチャ座標のv軸をずらす
	float tv;

	// スライダー用
	Vec2 max_uv_slider;

	// スライダー用
	Vec2 min_uv_slider;

	// ディフューズカラー
	DWORD color;

	// サンプラーステートの状態
	Graphics::SamplerStateType type;
};
