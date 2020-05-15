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
		const char * texture_name = "",
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

	// テクスチャポインタ
	IDirect3DTexture9*p_tex;

	// 調整用テクスチャサイズ(初期状態の画像の拡縮,通常は使わない)
	Vec2 tex_size;

	// 座標
	Vec2 pos;

	// 拡縮(累乗拡縮)
	Vec2 scale;

	// 角度
	float angle;

	// オフセット
	Vec2 offset;

	// アニメーション用のパラメータ
	AnimationParamter animation_param;

	bool is_uv_slider;

	// uv値(ずらす用)
	Vec2 uv;

	// スライダー用
	Vec2 max_uv_slider;

	// スライダー用
	Vec2 min_uv_slider;

	// ディフューズカラー
	DWORD color;

	// サンプラーステートの状態
	Graphics::SamplerStateType type;
};
