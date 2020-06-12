#pragma once
#include"../../../Graphics/Graphics.h"


/**
* @brief マテリアル構造体
*/
struct Material{

	Material() {
		ambient = D3DXVECTOR4(0.f, 0.f, 0.f, 0.f);
		diffuse = D3DXVECTOR4(0.f, 0.f, 0.f, 0.f);
		specular = D3DXVECTOR4(0.f, 0.f, 0.f, 0.f);
	}

	D3DXVECTOR4 ambient;
	D3DXVECTOR4 diffuse;
	D3DXVECTOR4 specular;
};


/**
* @brief 点光源データ
*/
struct PointLight {

	D3DXVECTOR4 diffuse;
	D3DXVECTOR4 specular;
	D3DXVECTOR4 pos;
	D3DXVECTOR4 attenuation;
};


/**
* @brief ライトを作るのに必要なデータ
*/
struct LightData {

	LightData() {

	}

	//! マテリアル
	Material material;

	//! 点光源
	PointLight point_light;

	//! ライトの方向
	D3DXVECTOR4 direction;

	//! 注視点座標
	D3DXVECTOR4 eye_direction;

	//! カメラ座標
	D3DXVECTOR4 eye_pos;

	//! ライトカラー値
	D3DXVECTOR4 light_color;

	//! 環境光
	D3DXVECTOR4 ambient;
};
