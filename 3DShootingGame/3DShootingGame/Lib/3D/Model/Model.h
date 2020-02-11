#pragma once
#include<vector>
#include"../../Graphics/Graphics.h"


// マテリアル情報構造体
struct MaterialInfo {
	D3DMATERIAL9 material;
	std::string texture_name;
};


class Model {
public:

	Model();

protected:

	// インデックスバッファ生成
	bool IndexBufferCreate(
		const int &total_face_num,
		std::vector<INT>&indices,
		LPDIRECT3DINDEXBUFFER9 * buffer
	);

	// 頂点バッファ生成
	bool VertexBufferCreate(
		const int &total_vertex,
		LPDIRECT3DVERTEXBUFFER9 * p_vertex_buffer
	);

	// パラメータのセット
	void Set3DParameter(
		Graphics*graphics,
		IDirect3DIndexBuffer9*p_i_buf,
		IDirect3DVertexBuffer9*p_v_buf,
		const UINT &buffer_size,
		const D3DMATERIAL9&material,
		const DWORD&fvf
	);

	// 通常描画
	void DrawPrimitive(
		const int &vertex_num,
		const int&polygon_num
	);

	// ワールド座標変換
	D3DXMATRIX WorldTransform(
		D3DXVECTOR3 &pos,
		D3DXVECTOR3 &rot,
		D3DXVECTOR3 &scale);


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

	// ポリゴン4を3にする
	std::vector<INT> Polygon4ToPolygon3Convert(
		const std::vector<INT>& vertex4_polygon_list);

protected:

	// グラフィックス
	Graphics*m_p_graphics;
};