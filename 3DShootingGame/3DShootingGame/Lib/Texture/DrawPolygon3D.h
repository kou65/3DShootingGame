#pragma once
#include"TextureManager.h"



struct TextureFormat3D;


// ポリゴンの向き
enum PolygonDirection {
	FRONT,
	BACK,
	RIGHT,
	LEFT,
	FLOOR
};


class DrawPolygon3D {
public:

	// アクセス権
	static DrawPolygon3D *GetInstance(){
		static DrawPolygon3D draw_polygon3d;
		return &draw_polygon3d;
	}

public:

	// 板形状で描画
	void BoardDraw(
		TextureFormat3D texture_format_3d
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