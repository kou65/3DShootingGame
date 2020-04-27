#pragma once
#include"../../../Texture/TextureManager/TextureManager.h"
#include"../Sprite3DParameter/Sprite3DParameter.h"




/**
* @brief Sprite用カスタムバーテックス
*/
struct CustomVertex3D {

	// 頂点情報
	D3DXVECTOR3 vertex;

	// 頂点カラー
	DWORD color;

	// テクスチャ座標
	D3DXVECTOR2 uv;
};


//! テンプレートFVF3D
#define FVF_3D (D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_DIFFUSE)


/**
* @brief 固定機能3Dスプライト使用者クラス
*/
class Sprite3DUser {
public:

	/**
	* @brief 板形状で描画
	*/
	void BoardDraw(
		Sprite3DParameter texture_format_3d
	);

private:


	/**
	* @brief 板形状形成
	* @param[out] custom_vertex3D カスタムバーテックス
	* @param[in] vertex_num 頂点数
	*/
	void ShapeBoard(
		CustomVertex3D * custom_vertex3D,
		int vertex_num,
		float right,
		float left,
		float top,
		float bottom,
		DWORD color
	);


	/**
	* @brief 行列演算
	*/
	D3DXMATRIX CalcMatrixTransform(
		D3DXVECTOR3 pos,
		D3DXVECTOR3 scale,
		PolygonDirection dir
	);


	/**
	* @brief 方向回転行列計算
	*/
	D3DXMATRIX CalcMatrixRotation(
		PolygonDirection dir
	);

};