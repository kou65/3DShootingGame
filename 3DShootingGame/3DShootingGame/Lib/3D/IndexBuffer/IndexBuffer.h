#pragma once
#include"../../Graphics/Graphics.h"
#include<vector>
#include"../VertexBuffer/VertexBuffer.h"



class IndexBuffer {
public:


	IndexBuffer(Graphics * graphics);


	bool Create(DWORD polygon_num);

	void Draw();

private:

	// コピーコンストラクタ禁止
	//IndexBuffer() = default;

	void *p_data;

	// インデックスバッファ
	IDirect3DIndexBuffer9 *m_p_index_buffer9;

	// バーテックスバッファ
	VertexBuffer *m_p_vertex_buffer;

	// DirectGraphics
	Graphics * graphics;
};