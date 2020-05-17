#ifndef SHADOW_DATA_H
#define SHADOW_DATA_H


#include<d3dx9.h>


struct ShadowData {

	// ライト用
	D3DXMATRIX light_view;
	D3DXMATRIX light_proj;

	// カメラ用
	D3DXMATRIX camera_view;
	D3DXMATRIX camera_proj;
};

#endif
