﻿#include"VertexBuffer.h"





VertexBuffer::VertexBuffer() {

	// グラフィック代入
	this->graphics = Graphics::GetInstance();
}


void VertexBuffer::Create(const UINT &buffer_size) {
	
	// 頂点バッファ作成
	graphics->GetDevice()->CreateVertexBuffer(
		// 頂点バッファサイズ(CustomVertex * 頂点数)
		buffer_size,
		// リソースの使用法
		D3DUSAGE_WRITEONLY,
		// 柔軟な頂点フォーマットの型を指定する
		D3DFMT_INDEX16,
		// 頂点バッファをどの種類のメモリに置くか
		D3DPOOL_MANAGED,
		// 頂点バッファ
		&mp_vertex_buffer9,
		// phandleは現在使用されていない
		NULL
	);

}



bool VertexBuffer::CloseBuffer() {

	mp_vertex_buffer9->Unlock();

	return true;
}


void VertexBuffer::SetStream(
	const UINT &size,
	const int &stream_number
) {

	// 頂点処理の流れに頂点バッファを実際にセットできる
	graphics->GetDevice()->SetStreamSource(
		stream_number,
		mp_vertex_buffer9,
		0,
		size
	);
}


void VertexBuffer::Draw() {

	if (mp_vertex_buffer9 == nullptr) {
		return;
	}

	graphics->GetDevice()->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2
	);
}



//bool VertexBuffer::OpenBufferIndex(
//	const UINT&buffer_size,
//	IndexCustomVertex*p_custom_vertex
//) {
//
//	// ロックして読み込み可能にさせる
//	HRESULT hr = mp_vertex_buffer9->Lock(
//		0,
//		buffer_size,
//		(void**)&p_custom_vertex,
//		0
//	);
//
//	if (hr != S_OK) {
//		return false;
//	}
//
//	return true;
//}



/*
bool VertexBuffer::Load() {


	const int VERTEX_NUM = 4;

	// 頂点情報埋め込み
	//CustomVertex v[VERTEX_NUM] =
	//{
	//{ 1.f, 0.f, 0.f,1.f,0xffffff,},// 0.f, 0.f },
	//{ 1.f, 1.f, 0.f,1.f,0xffffff,},// 0.f, 1.f },
	//{ 0.f, 0.f, 0.f,1.f,0xffffff,},// 1.f, 0.f },
	//{ 0.f, 1.f, 0.f,1.f,0xffffff,}// 1.f, 1.f }
	//};

	// リザルト変数
	HRESULT result;

	// 頂点バッファ作成
	result = graphics->GetLpDirect3DDevice9()->CreateVertexBuffer(
		// 頂点バッファサイズ(CustomVertex * 頂点数)
		sizeof(CustomVertex) * VERTEX_NUM,
		// リソースの使用法
		D3DUSAGE_WRITEONLY,
		// 柔軟な頂点フォーマットの型を指定する
		FVF_CUSTOM,
		// 頂点バッファをどの種類のメモリに置くか
		D3DPOOL_MANAGED,
		// 頂点バッファ
		&mp_vertex_buffer9,
		// phandleは現在使用されていない
		NULL
	);

	if (result != D3D_OK) {
		return false;
	}

	// バッファデータ
	void *p_data;

	// バッファデータをロック
	result = mp_vertex_buffer9->Lock(
		0,
		sizeof(CustomVertex) * VERTEX_NUM,
		(void**)&p_data,
		0
	);

	// 成功していなかったら失敗を返す
	if (result != D3D_OK) {
		return false;
	}

	// メモリコピー
	//memcpy(
	//	p_data,
	//	v,
	//	sizeof(CustomVertex) * VERTEX_NUM
	//);

	// ロック解除して使えるようにする
	mp_vertex_buffer9->Unlock();

	return true;
}
*/