#pragma once
#include<d3dx9.h>
#include<math.h>



namespace Math {



	/*
	@ brief 移動　回転　拡縮の行列変換を行う
	*/
	void OutTransformMatrix(
		D3DXMATRIX&total_mat,
		D3DXVECTOR3&pos,
		D3DXVECTOR3&scale,
		D3DXVECTOR3&rota
	);

	D3DXMATRIX GetTransformMatrix(
		D3DXVECTOR3&pos,
		D3DXVECTOR3&scale,
		D3DXVECTOR3&rota
	);

	void MatrixMultipleVec3(
		D3DXVECTOR3&out_vec3,
		const D3DXVECTOR3&base_vec3,
		const D3DXMATRIX&mat
	);

	void MatrixMultipleVec4(
		D3DXVECTOR4&out_vec3,
		const D3DXVECTOR4&base_vec3,
		const D3DXMATRIX&mat
	);
}