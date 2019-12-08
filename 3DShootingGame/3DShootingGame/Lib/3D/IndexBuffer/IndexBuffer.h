#pragma once
#include"../../Graphics/Graphics.h"
#include<vector>
#include"../VertexBuffer/VertexBuffer.h"



class IndexBuffer {
public:

	IndexBuffer(Graphics * graphics);

	bool Create(DWORD polygon_num);

	// バッファを開ける
	bool OpenBuffer(void**p_custom_vertex);

	// バッファを閉じる
	bool CloseBuffer();

	void Draw();

	operator IDirect3DIndexBuffer9*() const {
		return m_p_index_buffer9;
	}

private:

	// コピーコンストラクタ禁止
	//IndexBuffer() = default;

	// インデックスバッファ
	IDirect3DIndexBuffer9 *m_p_index_buffer9;

	// DirectGraphics
	Graphics * graphics;
};