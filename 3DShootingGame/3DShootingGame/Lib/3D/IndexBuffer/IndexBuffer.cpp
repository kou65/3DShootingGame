#include"IndexBuffer.h"
#include"../VertexBuffer/VertexBuffer.h"



IndexBuffer::IndexBuffer() {
	this->graphics = Graphics::GetInstance();
}


bool IndexBuffer::Create16(
	DWORD polygon_num
) {


	// インデックスバッファ作成
	graphics->GetDevice()->CreateIndexBuffer(
		// インデックスバッファのサイズをバイト単位で指定
		polygon_num * sizeof(WORD),
		// 頂点バッファをどのように使用するか
		D3DUSAGE_WRITEONLY,
		// 一つのインデックスバッファのサイズをフラグで表す
		D3DFMT_INDEX16,
		// 頂点インデックスをどのメモリに置くか指定
		D3DPOOL_MANAGED,
		// IDirect3DIndexBuffer9インターフェースが返る
		&m_p_index_buffer9,
		// 現在使用されていないので基本NULL
		NULL
	);

	return true;
}



// バッファを閉じる
bool IndexBuffer::CloseBuffer() {


	// nullチェック
	if (m_p_index_buffer9 == nullptr) {
		return false;
	}

	// アンロック
	m_p_index_buffer9->Unlock();

	return true;
}


void IndexBuffer::SetIndices() {

	// インデックス番号をデバイスに設定する
	graphics->GetDevice()->SetIndices(
		m_p_index_buffer9
	);
}



// バッファを開ける
//bool IndexBuffer::OpenBuffer(void**p_custom_vertex) {
//
//
//	if (FAILED(m_p_index_buffer9->Lock(
//		// ロックしたい位置をバイト単位で指定する
//		0,
//		// ロックするサイズをバイト単位で指定する
//		0,
//		// 指定した頂点インデックスバッファへのポインタが返る
//		(void**)&p_custom_vertex,
//		// ロック目的をフラグで示す(大抵は節約なくロックする)
//		D3DLOCK_DISCARD
//	)
//	)
//		) {
//		return false;
//	}
//
//	return true;
//}

