#pragma once
#include"../../Graphics/Graphics.h"


// fvf
#define FVF_SKIN ( D3DFVF_XYZW | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_DIFFUSE /*| D3DFVF_LASTBETA_UBYTE4 | D3DFVF_XYZB4*/)


struct SkinCustomVertex {

	// コンストラクタあり
	SkinCustomVertex();

	// 頂点データ
	D3DXVECTOR4 vertex;

	// 法線ベクトル(現在はなしなので、戻す場合はfbxの処理も戻すようにする)
	D3DXVECTOR3 normal;

	// カラー
	DWORD diffuse;

	// テクセル
	D3DXVECTOR2 uv;

	// スキンメッシュ用影響するボーン番号(頂点)
	unsigned char bone_index[4];

	// スキンメッシュ用重み(この頂点で影響する重みを全て持たせる)
	D3DXVECTOR4 weight;
};
