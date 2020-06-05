#pragma once
#include<d3dx9.h>



/**
* @brief zテクスチャのデータ
*/
struct ZTextureData {

	ZTextureData() {
		D3DXMatrixIdentity(&mat_camera_view);
		D3DXMatrixIdentity(&mat_camera_proj);
	}

	D3DXMATRIX mat_camera_view;
	D3DXMATRIX mat_camera_proj;
};