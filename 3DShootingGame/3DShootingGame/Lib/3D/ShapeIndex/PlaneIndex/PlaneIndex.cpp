#include"../PlaneIndex/PlaneIndex.h"



PlaneIndex::PlaneIndex() {

	Create();
	m_ns.Init();
}


void PlaneIndex::Create() {

	VertexBufCreate();

	IndexBufCreate();
}


void PlaneIndex::Draw(const IndexShapeParameter &param) {

	// シェーダー更新
	m_ns.Update();

	// パス
	UINT pass;

	// シェーダーの描画開始
	m_ns.ShaderBegin(pass, 0);

	//m_vertex_buf.SetStream(sizeof(IndexCustomVertex));

	// 頂点バッファの設定
Graphics::GetInstance()->GetDevice()->SetStreamSource(
		0,
	// オペレーター発動
		m_vertex_buf,
		0,
		// 1頂点当たりの大きさ
		sizeof(IndexCustomVertex)
	);

	// インデックスバッファの設定
	m_idx_buf.SetIndices();

	m_ns.BeginPass();

	Graphics::GetInstance()
		->GetDevice()
		->DrawIndexedPrimitive(
		// 頂点のつなぎ方
		D3DPT_TRIANGLELIST,
		// 頂点インデックスの一番最初までのオフセット値を指定
		0,
		// 描画に使用する最小のインデックス番号を指定(多少無駄にしていいなら0)
		0,
		// 上の引数の最小以降の頂点数を指定
		PLANE_VERTEX,
		// 描画を開始する頂点インデックスまでのオフセット値を指定
		0,
		// 上引数を先頭として描画するポリゴンの数を指定する(ポリゴンの数,頂点の数ではない)
		POLYGON_NUM
	);

	m_ns.EndPass();

	m_ns.ShaderEnd();
}


void PlaneIndex::VertexBufCreate() {

	IndexCustomVertex*p_cv = nullptr;
	UINT size = PLANE_VERTEX * sizeof(IndexCustomVertex);

	m_vertex_buf.Create(size);

	// ロックして読み込み可能にさせる
	HRESULT hr = m_vertex_buf.GetBufferPtr()->Lock(
		0,
		size,
		(void**)&p_cv,
		0
	);

	// 左上
	p_cv[0].vertex.x = 0.f;
	p_cv[0].vertex.y = 0.f;
	p_cv[0].uv.x = 0.f;
	p_cv[0].uv.y = 0.f;

	// 右上
	p_cv[1].vertex.x = 1.f;
	p_cv[1].vertex.y = 0.f;
	p_cv[1].uv.x = 1.f;
	p_cv[1].uv.y = 0.f;

	// 左下
	p_cv[2].vertex.x = 0.f;
	p_cv[2].vertex.y = 1.f;
	p_cv[2].uv.x = 0.f;
	p_cv[2].uv.y = 1.f;

	// 右下
	p_cv[3].vertex.x = 1.f;
	p_cv[3].vertex.y = 1.f;
	p_cv[3].uv.x = 1.f;
	p_cv[3].uv.y = 1.f;

	m_vertex_buf.CloseBuffer();
}


void PlaneIndex::IndexBufCreate() {

	m_idx_buf.Create16(POLYGON_NUM);

	WORD * p_idx = nullptr;

	m_idx_buf.GetBufferPtr()->Lock(
		// ロックしたい位置をバイト単位で指定する
		0,
		// ロックするサイズをバイト単位で指定する
		0,
		// 指定した頂点インデックスバッファへのポインタが返る
		(void**)&p_idx,
		// ロック目的をフラグで示す(大抵は節約なくロックする)
		D3DLOCK_DISCARD
	);

	// 1ポリ
	p_idx[0] = 0;
	p_idx[1] = 1;
	p_idx[2] = 2;

	// 2ポリ
	p_idx[3] = 2;
	p_idx[4] = 3;
	p_idx[5] = 1;

	m_idx_buf.CloseBuffer();
}