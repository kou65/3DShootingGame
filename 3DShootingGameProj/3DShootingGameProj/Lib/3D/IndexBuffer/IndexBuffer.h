#pragma once
#include"../../Graphics/Graphics.h"
#include<vector>
#include"../VertexBuffer/VertexBuffer.h"


/**
* @brief インデックス描画する為のインデックスクラス
*/
class IndexBuffer {
public:


	/**
	* @brief コンストラクタ
	*/
	IndexBuffer();

	/**
	* @brief 頂点16bitで生成
	* @return bool 生成できたかどうか
	*/
	bool Create16(DWORD polygon_num);


	/**
	* @brief バッファを閉じる
	*/
	bool CloseBuffer();


	/**
	* @brief インデックスをセット
	*/
	void SetIndices();


	/**
	* @brief index自動ポインタ返し
	* @return IDirect3DIndexBuffer9*
	*/
	operator IDirect3DIndexBuffer9*() const {
		return m_p_index_buffer9;
	}


	/**
	* @brief indexポインタ返し
	* @return IDirect3DIndexBuffer9*
	*/
	IDirect3DIndexBuffer9*GetBufferPtr() {
		return m_p_index_buffer9;
	}

	// バッファを開ける
	//bool OpenBuffer(void**p_custom_vertex);

private:

	// コピーコンストラクタ禁止
	//IndexBuffer() = default;

	//! インデックスバッファ
	IDirect3DIndexBuffer9 *m_p_index_buffer9;

	//! DirectGraphics
	Graphics * graphics;
};