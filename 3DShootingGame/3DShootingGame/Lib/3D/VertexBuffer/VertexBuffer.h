#pragma once
#include"../../Graphics/Graphics.h"
#include"../Object3DCustomVertex/MeshCustomVertex.h"

/* 頂点バッファを使うメリット

VRAM上に頂点情報を送れる

*/



class VertexBuffer {
public:

	// 独自の頂点バッファを作る場合のコンストラクタ
	VertexBuffer(int vertex_num);

	// 正方形頂点バッファ
	VertexBuffer();

	// 描画
	void Draw();

	operator IDirect3DVertexBuffer9*() const {
		return m_p_vertex_buffer9;
	}

	// 頂点バッファ作成
	void Create(const UINT &buffer_size);

	// バッファを開ける
	bool OpenBuffer(
		const UINT&buffer_size,
		void**p_custom_vertex
	);

	// バッファを閉じる
	bool CloseBuffer();

private:

	// バーテックスバッファ
	IDirect3DVertexBuffer9 * m_p_vertex_buffer9;

	// DirectGraphics
	Graphics * graphics;

};