#pragma once
#include"Lib/D3D9/D3D9.h"
#include<vector>


struct CustomVertex {
	// 頂点
	D3DXVECTOR3 vertex;
	// 法線
	D3DXVECTOR3 normal;
	// テクスチャ座標
	D3DXVECTOR2 uv;
};


class IndexBuffer {
	

	bool Create(DWORD polygon_num) {

		// インデックスバッファ作成
		D3D9::GetLpDirect3DDevice9()->CreateIndexBuffer(
			// インデックスバッファのサイズをバイト単位で指定
			polygon_num * 3,
			// 頂点バッファをどのように使用するか
			D3DUSAGE_DYNAMIC,
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


		p_index_buffer9->Lock(
			// ロックしたい位置をバイト単位で指定する
			0,
			// ロックするサイズをバイト単位で指定する
			0,
			// 指定した頂点インデックスバッファへのポインタが返る
			&p_data,
			// ロック目的をフラグで示す(大抵は節約なくロックする)
			D3DLOCK_DISCARD
		);

		// nullチェック
		if (p_data == nullptr) {
			return false;
		}
		
		// アンロック
		p_index_buffer9->Unlock();

		return true;
	}


	void Draw() {

		// ストリームをセット
		D3D9::GetLpDirect3DDevice9()->SetStreamSource(
			0,
			p_vertex_buffer9,
			D3DDEVCAPS2_DMAPNPATCH,
			sizeof(IDirect3DVertexBuffer9)
		);

		// どの情報を伝えるか
		D3D9::GetLpDirect3DDevice9()->SetFVF(D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_XYZ);

		// インデックス番号をデバイスに設定する
		D3D9::GetLpDirect3DDevice9()->SetIndices(
			p_index_buffer9
		);

		// 仮
		int vertex_num = 1;

		D3D9::GetLpDirect3DDevice9()->DrawIndexedPrimitive(
			// 頂点のつなぎ方
			D3DPT_TRIANGLELIST,
			// 頂点インデックスの一番最初までのオフセット値を指定
			0,
			// 描画に使用する最小のインデックス番号を指定(多少無駄にしていいなら0)
			0,
			// 上引数の最小以降の頂点数を指定
			vertex_num,
			// 描画を開始する頂点インデックスまでのオフセット値を指定
			0,
			// 上引数を先頭として描画するポリゴンの数を指定する(ポリゴンの数,頂点の数ではない)
			vertex_num * 3
			);
	}

private:

	void *p_data;

	// インデックスバッファ
	IDirect3DIndexBuffer9 *p_index_buffer9;

	// バーテックスバッファ
	IDirect3DVertexBuffer9 * p_vertex_buffer9;

};