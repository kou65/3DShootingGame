#pragma once
#include"../../Graphics/Graphics.h"
#include"../Object3DCustomVertex/MeshCustomVertex.h"

/* 頂点バッファを使うメリット

VRAM上に頂点情報を送れる

*/



class VertexBuffer {
public:

	// 形状
	enum Shape{
		NONE,
		BOX,
	};

public:

	// 独自の頂点バッファを作る場合のコンストラクタ
	VertexBuffer(int vertex_num);

	// 正方形頂点バッファ
	VertexBuffer(Shape shape);

	// 描画
	void Draw();

	operator IDirect3DVertexBuffer9*() const {
		return m_p_vertex_buffer9;
	}

private:

	// 頂点バッファ作成
	void Create(int vertex_num,Shape shape);

	// 正方形形状に頂点作成する
	void Cube(MeshCustomVertex v[8]);

private:

	// バーテックスバッファ
	IDirect3DVertexBuffer9 * m_p_vertex_buffer9;

	// DirectGraphics
	Graphics * graphics;

};