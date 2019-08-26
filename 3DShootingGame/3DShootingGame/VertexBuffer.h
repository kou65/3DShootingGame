#pragma once
#include"Lib/Graphics/Graphics.h"


/* 頂点バッファを使うメリット

VRAM上に頂点情報を送れる

*/


struct CustomVertex {
	// 頂点
	D3DXVECTOR3 vertex;
	// 法線
	//D3DXVECTOR3 normal;
	
	CustomVertex() {

		vertex.x = vertex.y = vertex.z = 0.f;
	}
};


#define FVF_CUSTOM (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)


class VertexBuffer {
public:

	VertexBuffer();

	void Create(int vertex_num,int pos_x, int pos_y, int pos_z);


	//bool Load();

	// 生成
	bool Draw();
	
	operator IDirect3DVertexBuffer9*() const {
		return m_p_vertex_buffer9;
	}

private:

	// バーテックスバッファ
	IDirect3DVertexBuffer9 * m_p_vertex_buffer9;

	// DirectGraphics
	Graphics * graphics;

};