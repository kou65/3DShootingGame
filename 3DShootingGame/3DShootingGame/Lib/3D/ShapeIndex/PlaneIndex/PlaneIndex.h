#pragma once
#include"../../../3D/IndexBuffer/IndexBuffer.h"
#include"../../../3D/VertexBuffer/VertexBuffer.h"
#include"../IndexCustomVertex/IndexCustomVertex.h"
#include"../../../EffectFileShader/NormalShader.h"
#include"../IndexShapeParameter/IndexShapeParameter.h"



class PlaneIndex {
public:

	PlaneIndex();

	void Draw(const IndexShapeParameter &param);

private:

	// 4頂点
	const int PLANE_VERTEX = 4;

	// ポリゴンの数2
	const int POLYGON_NUM = 2;

	// 頂点データ数4
	const int VERTEX_DATA_NUM = 4;

private:

	void Create();

	void VertexBufCreate();
	void IndexBufCreate();

private:

	IndexBuffer m_idx_buf;
	VertexBuffer m_vertex_buf;
	NormalShader m_ns;
};