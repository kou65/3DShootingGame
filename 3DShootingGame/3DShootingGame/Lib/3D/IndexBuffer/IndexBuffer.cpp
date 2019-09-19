#include"IndexBuffer.h"
#include"../VertexBuffer/VertexBuffer.h"



IndexBuffer::IndexBuffer(Graphics * graphics) {
	this->graphics = graphics;

	m_p_vertex_buffer = new VertexBuffer(VertexBuffer::BOX);
}


bool IndexBuffer::Create(DWORD polygon_num) {


	// インデックスバッファ作成
	graphics->GetLpDirect3DDevice9()->CreateIndexBuffer(
		// インデックスバッファのサイズをバイト単位で指定
		polygon_num * sizeof(WORD) * TOTAL_VERTEX_TYPE,
		// 頂点バッファをどのように使用するか
		D3DUSAGE_WRITEONLY,//D3DUSAGE_DYNAMIC,
		// 一つのインデックスバッファのサイズをフラグで表す
		D3DFMT_INDEX16,
		// 頂点インデックスをどのメモリに置くか指定
		D3DPOOL_MANAGED,
		// IDirect3DIndexBuffer9インターフェースが返る
		&m_p_index_buffer9,
		// 現在使用されていないので基本NULL
		NULL
	);

	// nullチェック
	if (m_p_index_buffer9 == nullptr) {
		return false;
	}


	WORD * buffer;

	if (FAILED(m_p_index_buffer9->Lock(
		// ロックしたい位置をバイト単位で指定する
		0,
		// ロックするサイズをバイト単位で指定する
		0,
		// 指定した頂点インデックスバッファへのポインタが返る
		(void**)&buffer,
		// ロック目的をフラグで示す(大抵は節約なくロックする)
		D3DLOCK_DISCARD
	)
	)
		) {
		return false;
	}


	// インデックスをセット

	// 表面
	{
		buffer[0] = 0;
		buffer[1] = 2;
		buffer[2] = 1;

		buffer[3] = 2;
		buffer[4] = 3;
		buffer[5] = 1;
	}

	// 裏面
	{
		buffer[6] = 5;
		buffer[7] = 6;
		buffer[8] = 4;

		buffer[9] = 5;
		buffer[10] = 7;
		buffer[11] = 6;
	}

	// nullチェック
	if (p_data == nullptr) {
		return false;
	}

	// アンロック
	m_p_index_buffer9->Unlock();

	return true;
}



void IndexBuffer::Draw() {

	// ワールド座標初期化
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	graphics->GetInstance()->GetLpDirect3DDevice9()
		->SetTransform(D3DTS_WORLD, &mat);


	// ストリームをセット
	graphics->GetLpDirect3DDevice9()->SetStreamSource(
		0,
		*m_p_vertex_buffer,
		0,
		sizeof(MeshCustomVertex)
	);


	// インデックス番号をデバイスに設定する
	graphics->GetLpDirect3DDevice9()->SetIndices(
		m_p_index_buffer9
	);

	// どの情報を伝えるか
	graphics->GetLpDirect3DDevice9()->SetFVF(FVF_CUSTOM);

	graphics->GetLpDirect3DDevice9()->DrawIndexedPrimitive(
		// 頂点のつなぎ方
		D3DPT_TRIANGLELIST,
		// 頂点インデックスの一番最初までのオフセット値を指定
		0,
		// 描画に使用する最小のインデックス番号を指定(多少無駄にしていいなら0)
		0,
		// 上引数の最小以降の頂点数を指定
		8,
		// 描画を開始する頂点インデックスまでのオフセット値を指定
		0,
		// 上引数を先頭として描画するポリゴンの数を指定する(ポリゴンの数,頂点の数ではない)
		16
	);
}