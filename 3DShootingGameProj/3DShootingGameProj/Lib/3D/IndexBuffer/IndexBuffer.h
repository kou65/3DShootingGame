#pragma once
#include"../../Graphics/Graphics.h"
#include<vector>
#include"../VertexBuffer/VertexBuffer.h"



class IndexBuffer {
public:

	IndexBuffer();

	// 頂点データの数(頂点、法線、色など)
	bool Create16(DWORD polygon_num);

	// バッファを閉じる
	bool CloseBuffer();

	// インデックスをセット
	void SetIndices();

	operator IDirect3DIndexBuffer9*() const {
		return m_p_index_buffer9;
	}

	// ポインタ返し
	IDirect3DIndexBuffer9*GetBufferPtr() {
		return m_p_index_buffer9;
	}

	// バッファを開ける
	//bool OpenBuffer(void**p_custom_vertex);

private:

	// コピーコンストラクタ禁止
	//IndexBuffer() = default;

	// インデックスバッファ
	IDirect3DIndexBuffer9 *m_p_index_buffer9;

	// DirectGraphics
	Graphics * graphics;
};