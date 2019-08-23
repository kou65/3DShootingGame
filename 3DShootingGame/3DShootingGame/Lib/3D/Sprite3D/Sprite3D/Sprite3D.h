#pragma once
#include"../../../Texture/TextureManager/TextureManager.h"
#include"../Sprite3DData/Sprite3DData.h"



class Sprite3D {
public:

	// アクセス権
	static Sprite3D *GetInstance(){
		static Sprite3D draw_polygon3d;
		return &draw_polygon3d;
	}

public:

	// 板形状で描画
	void BoardDraw(
		Sprite3DData texture_format_3d
	);

private:

	// 行列演算
	D3DXMATRIX CalcMatrixTransform(
		D3DXVECTOR3 pos,
		D3DXVECTOR3 scale,
		PolygonDirection dir
	);

	// 方向回転行列計算
	D3DXMATRIX CalcMatrixRotation(
		PolygonDirection dir
	);
};