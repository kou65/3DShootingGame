#pragma once
#include<vector>
#include"../../Graphics/Graphics.h"



class Model {
protected:


	// インデックスバッファ生成
	bool IndexBufferCreate(
		int total_face_num,
		std::vector<INT>indices,
		LPDIRECT3DINDEXBUFFER9 * buffer
	);

	// 頂点バッファ生成
	bool VertexBufferCreate(
		int total_vertex,
		LPDIRECT3DVERTEXBUFFER9 * p_vertex_buffer
	);


	// ワールド座標変換
	D3DXMATRIX WorldTransform(
		D3DXVECTOR3 &pos,
		D3DXVECTOR3 &rot,
		D3DXVECTOR3 &scale);


	// ポリゴン4を3にする
	std::vector<INT> Polygon4ToPolygon3Convert(
		const std::vector<INT>& vertex4_polygon_list);


};