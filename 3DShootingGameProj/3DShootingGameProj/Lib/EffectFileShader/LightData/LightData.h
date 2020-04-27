#pragma once
#include"../../Graphics/Graphics.h"



struct LightData {

	// ライトの方向
	D3DXVECTOR4 direction;

	// 点ライト位置
	D3DXVECTOR4 point_light_pos;

	// 点光源の明るさ減衰
	D3DXVECTOR4 attenuation;

	// 注視点座標
	D3DXVECTOR4 eye_dir;

	// カメラ座標
	D3DXVECTOR4 eye_pos;

	// ライトカラー値
	D3DXVECTOR4 light_color;

	// アンビエント色
	D3DXVECTOR4 ambient_color;
};
