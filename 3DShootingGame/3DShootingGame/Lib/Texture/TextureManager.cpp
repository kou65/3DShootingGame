#include"TextureManager.h"
#include"../D3D9/D3D9.h"



TextureManager::~TextureManager() {

}


void TextureManager::Load2D(
	const char* file_name,
	const char* texture_rename,
	UINT width,
	UINT height,
	float tu,
	float tv
) {

	// テクスチャデータを生成
	m_texture_data2D_list[texture_rename] = new TextureData2D;

	// 画像サイズを保存するパラメーター
	D3DXIMAGE_INFO info;

	// リザルト
	HRESULT result;

	// ファイルサイズの読み込み
	if (FAILED(D3DXGetImageInfoFromFile(file_name, &info))) {

		// 読み込みエラー
		MessageBoxA(0, "テクスチャのファイルサイズ読み込みに失敗しました", TEXT("MessageBox"), MB_OK);
		return;
	}


	// 最初はサイズ指定をしなければいけない。
	m_texture_data2D_list[texture_rename]->width_size
		= (float)info.Width;
	m_texture_data2D_list[texture_rename]->height_size
		= (float)info.Height;

	// UV設定
	m_texture_data2D_list[texture_rename]->uv.x
		= tu;
	m_texture_data2D_list[texture_rename]->uv.y
		= tv;


	// EXバージョンの読み込み
	result = D3DXCreateTextureFromFileEx(
		// window_device
		D3D9::GetLpDirect3DDevice9(),
		// ファイル名
		file_name,
		// 読み込むファイル幅
		info.Width + width,
		// 読み込むファイル縦幅
		info.Height + height,
		// ミップマップレベル
		1,
		// テクスチャの使い方
		0,
		// カラーフォーマット
		D3DFMT_UNKNOWN,
		// テクスチャのメモリ管理方法
		D3DPOOL_MANAGED,
		// フィルタリング方法
		D3DX_DEFAULT,
		// ミップマップのフィルタリング方法
		D3DX_DEFAULT,
		// カラー
		NULL,
		NULL,
		NULL,
		// テクスチャバッファ
		&m_texture_data2D_list[texture_rename]->p_texture_buffer);

	// 読み込みの失敗
	if (FAILED(result)) {
		return;
	}
}


void TextureManager::Load3D(
	const char*file_name,
	const char*texture_name,
	const int &texture_num
) {

	HRESULT result;

	// テクスチャ生成
	m_texture_data3D_list[texture_name] = new TextureData3D;

	result = D3DXCreateTextureFromFile(
		D3D9::GetLpDirect3DDevice9(),
		file_name,
		&m_texture_data3D_list[texture_name]->texture_list[texture_num]
	);

	// 読み込み失敗
	if (FAILED(result)) {
		return;
	}

}


bool TextureManager::Find2DTexture(std::string name) {

	if (m_texture_data2D_list.count(name.c_str()) == 0) {
		// ファイルが見つからない
		return false;
	}
	// ファイルが存在する
	return true;
}


bool TextureManager::Find3DTexture(std::string name) {

	if (m_texture_data3D_list.count(name.c_str()) == 0) {
		// ファイルが見つからない
		return false;
	}
	// ファイルが存在する
	return true;
}


TextureData2D &TextureManager::GetTextureData2D(std::string name) {
	return *m_texture_data2D_list[name.c_str()];
}


TextureData3D &TextureManager::GetTextureData3D(std::string name) {
	return *m_texture_data3D_list[name.c_str()];
}


void TextureManager::Release() {

	// 2Dテクスチャの解放
	for (auto texture : m_texture_data2D_list) {

		// テクスチャが存在しているなら
		if (texture.second->p_texture_buffer) {

			// 解放
			texture.second->p_texture_buffer->Release();
		}

		// 要素のメモリ削除
		delete[] & texture;
	}

	// 3Dテクスチャの解放
	for (auto texture : m_texture_data3D_list) {

		for (auto t : texture.second->texture_list) {

			delete[] t;
		}
	}
}
