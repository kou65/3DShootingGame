
#ifndef DEFAULT_MESH_H
#define DEFAULT_MESH_H

#include"../../Graphics/Graphics.h"



/**
* @brief DirectX標準のメッシュを描画 
*/
class D3DDefaultMesh {
public:


	/**
	* @brief コンストラクタ
	*/
	D3DDefaultMesh();


	/**
	* @brief セットしてある描画データをすべて描画
	*/
	void AllDraw();


	/**
	* @brief 描画
	* @param[in] p_mesh D3Dメッシュ
	* @param[in] pos 位置
	*/
	void Draw(const LPD3DXMESH &p_mesh,const D3DXVECTOR3 &pos);

private:


	/**
	* @brief ティーポット生成
	*/
	void CreateTeapot();


	/**
	* @brief トーラス生成
	*/
	void CreateTorus();


	/**
	* @brief コーン生成
	*/
	void CreateCone();

private:

	//! メッシュ1
	LPD3DXMESH mp_teapot;

	//! メッシュ2
	LPD3DXMESH mp_tours;

	//! メッシュ3
	LPD3DXMESH mp_cone;
};

#endif