#pragma once
#include"../../Graphics/Graphics.h"
#include"../Object3DCustomVertex/MeshCustomVertex.h"
#include"../../3D/ShapeIndex/IndexCustomVertex/IndexCustomVertex.h"

/* 頂点バッファを使うメリット

VRAM上に頂点情報を送れる

*/



class VertexBuffer {
public:

	// 正方形頂点バッファ
	VertexBuffer();

	// 描画
	void Draw();

	// 頂点バッファ作成
	void Create(const UINT &buffer_size);

	//// バッファを開ける
	//bool OpenBufferIndex(
	//	const UINT&buffer_size,
	//	IndexCustomVertex*p_custom_vertex
	//);

	// バッファを閉じる
	bool CloseBuffer();

	// ストリームセット
	void SetStream(
		const UINT &custom_vertex_size,
		const int &stream_number = 0
	);


	operator IDirect3DVertexBuffer9*() const {
		return mp_vertex_buffer9;
	}

	IDirect3DVertexBuffer9*GetBufferPtr() {
		return mp_vertex_buffer9;
	}


private:

	// バーテックスバッファ
	IDirect3DVertexBuffer9 * mp_vertex_buffer9;

	// DirectGraphics
	Graphics * graphics;

};