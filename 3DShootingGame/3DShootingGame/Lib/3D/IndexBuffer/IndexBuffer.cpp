#include"IndexBuffer.h"
#include"../../../VertexBuffer.h"




bool IndexBuffer::Create(DWORD polygon_num) {

	
	// 頂点バッファ作成
	vertex_buffer.Create(0, 0, 0, 0);


	WORD * i;

	// インデックスバッファ作成
	graphics->GetLpDirect3DDevice9()->CreateIndexBuffer(
		// インデックスバッファのサイズをバイト単位で指定
		polygon_num * sizeof(WORD) * 4,
		// 頂点バッファをどのように使用するか
		D3DUSAGE_WRITEONLY,//D3DUSAGE_DYNAMIC,
		// 一つのインデックスバッファのサイズをフラグで表す
		D3DFMT_INDEX16,
		// 頂点インデックスをどのメモリに置くか指定
		D3DPOOL_MANAGED,
		// IDirect3DIndexBuffer9インターフェースが返る
		&p_index_buffer9,
		// 現在使用されていないので基本NULL
		NULL
	);

	// nullチェック
	if (p_index_buffer9 == nullptr) {
		return false;
	}

	if (FAILED(p_index_buffer9->Lock(
		// ロックしたい位置をバイト単位で指定する
		0,
		// ロックするサイズをバイト単位で指定する
		0,
		// 指定した頂点インデックスバッファへのポインタが返る
		(void**)&i,
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
		i[0] = 0;
		i[1] = 2;
		i[2] = 1;

		i[3] = 2;
		i[4] = 3;
		i[5] = 1;
	}

	// 裏面
	{
		i[6] = 4;
		i[7] = 6;
		i[8] = 5;

		i[9] = 6;
		i[10] = 7;
		i[11] = 5;
	}

	// nullチェック
	if (p_data == nullptr) {
		return false;
	}

	// アンロック
	p_index_buffer9->Unlock();

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
		vertex_buffer,
		0,
		sizeof(CustomVertex)
	);


	// インデックス番号をデバイスに設定する
	graphics->GetLpDirect3DDevice9()->SetIndices(
		p_index_buffer9
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