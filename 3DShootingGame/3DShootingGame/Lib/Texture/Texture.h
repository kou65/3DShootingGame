﻿#pragma once

#include<d3dx9.h>
#include<unordered_map>

#pragma comment(lib,"d3dx9.lib")


/* 注意!
テクスチャの読み込みサイズは2の累乗でなければいけない
*/


// テクスチャデータを格納する。
struct TextureData
{
	TextureData();

	// tagTextureDataのオペレーター
	operator LPDIRECT3DTEXTURE9() const;

	// テクスチャパラメーター
	LPDIRECT3DTEXTURE9 Texture;
	float Width;
	float Height;
	D3DXVECTOR2 Uv;
};



namespace Texture {


	enum SamplerStateType {
		CLAMP,              // つなぎ目なし
		MIRROR,				// 反転
	};


	// テクスチャのロード
	void Load(const char*file_name);

	/* ---Exのロード--- 
	
	fila_name = 読み込むファイル名
	width = 読み込むサイズ横
	height = 読み込むサイズ縦
	u = UVのU軸の初期化
	v = UVのV軸の初期化
	
	*/
	void LoadEx(
		const char * file_name,
		UINT width_size = 0.f,
		UINT height_size = 0.f,
		float u = 0.f,
		float v = 0.f
	);

	// 解放
	void Release();

	// テクスチャデータのゲッター
	TextureData GetData(const std::string&texture_file_name);

	// テクスチャが登録されているか
	bool IsRedistr(const std::string&file_name);

	// サンプラーステートを設定
	bool SetSamplerStateSelect(Texture::SamplerStateType type);
}