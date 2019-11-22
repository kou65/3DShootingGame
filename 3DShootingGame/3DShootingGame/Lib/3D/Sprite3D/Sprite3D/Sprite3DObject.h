#pragma once
#include"../../../Texture/TextureManager/TextureManager.h"
#include"../Sprite3DParameter/Sprite3DParameter.h"



// MEMO 回転順序は固定


struct CustomVertex3D {

	// 頂点情報
	D3DXVECTOR3 vertex;

	// 頂点カラー
	DWORD color;

	// テクスチャ座標
	D3DXVECTOR2 uv;
};


// テンプレートFVF3D
#define FVF_3D (D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_DIFFUSE)


class Sprite3DObject {
public:

	// 板形状で描画
	void BoardDraw(
		Sprite3DData texture_format_3d
	);

private:

	// 板形状
	void ShapeBoard(
		CustomVertex3D * custom_vertex3D,
		int vertex_num,
		float right,
		float left,
		float top,
		float bottom,
		DWORD color
	);

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