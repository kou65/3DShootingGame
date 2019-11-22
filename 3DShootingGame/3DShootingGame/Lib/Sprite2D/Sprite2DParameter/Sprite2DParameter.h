#pragma once
#include"../../Graphics/Graphics.h"
#include<Windows.h>
#include<string>
#include"../../Uv/AnimationParameter/AnimationParameter.h"


// テクスチャのプロパティを設定する構造体



struct Sprite2DData {


	// コンストラクタ
	Sprite2DData(
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
		Graphics::SamplerStateType state_type = Graphics::CLAMP
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

	// アニメーション用のパラメータ
	AnimationParamter animation_param;

	// テクスチャ座標のu軸をずらす
	float tu;

	// テクスチャ座標のv軸をずらす
	float tv;

	// ディフューズカラー
	DWORD color;

	// サンプラーステートの状態
	Graphics::SamplerStateType type;
};
