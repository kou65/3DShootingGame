#ifndef SHADOW_DATA_H
#define SHADOW_DATA_H


#include<d3dx9.h>


/**
* @brief シャドウデータ
*/
struct ShadowData {

	ShadowData() {
	}

	//! カメラ用
	D3DXMATRIX camera_view_mat;
	D3DXMATRIX camera_proj_mat;

	//! ライト用
	D3DXMATRIX light_view_mat;
	D3DXMATRIX light_proj_mat;
};

#endif
