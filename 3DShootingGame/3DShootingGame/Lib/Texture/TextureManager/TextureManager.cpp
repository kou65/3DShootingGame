#include"TextureManager.h"
#include"../../Graphics/Graphics.h"



TextureManager::~TextureManager() {

}


void TextureManager::Load(
	const char* file_name,
	const char* texture_rename,
	UINT width,
	UINT height,
	float tu,
	float tv
) {

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

	// テクスチャデータを生成
	//m_texture_data2D_list[texture_rename] = new TextureData;
	m_texture_data2D_list[texture_rename].reset(new TextureData);

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
		Graphics::GetInstance()->GetDevice(),
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


void TextureManager::LoadMultiple(
	const char*file_name,
	const char*texture_name,
	const int &texture_num
) {

	HRESULT result;

	// テクスチャ生成
	m_texture_data3D_list[texture_name].reset(new TextureMultipleData);

	// 要素追加
	m_texture_data3D_list[texture_name]->texture_list.emplace_back();

	result = D3DXCreateTextureFromFile(
		Graphics::GetInstance()->GetDevice(),
		file_name,
		&m_texture_data3D_list[texture_name]->texture_list[texture_num]
	);

	// 読み込み失敗
	if (FAILED(result)) {
		return;
	}

}


bool TextureManager::FindTexture(std::string name) {

	if (m_texture_data2D_list.count(name.c_str()) == 0) {
		// ファイルが見つからない
		return false;
	}
	// ファイルが存在する
	return true;
}


bool TextureManager::FindMultipleTexture(std::string name) {

	if (m_texture_data3D_list.count(name.c_str()) == 0) {
		// ファイルが見つからない
		return false;
	}
	// ファイルが存在する
	return true;
}


TextureData &TextureManager::GetTextureData(std::string name) {
	return *m_texture_data2D_list[name.c_str()];
}


TextureMultipleData &TextureManager::GetTextureDataMultiple(std::string name) {
	return *m_texture_data3D_list[name.c_str()];
}


void TextureManager::AllRelease() {

	// 2Dテクスチャの解放
	for(auto itr = m_texture_data2D_list.begin();
		itr!=m_texture_data2D_list.end();
		itr++
		){

		// テクスチャが存在しているなら
		if (itr->second->p_texture_buffer) {

			// 解放
			itr->second->p_texture_buffer->Release();
			itr->second.reset();
		}
	}

	// 要素の解放
	m_texture_data2D_list.clear();

	// 3Dテクスチャの解放
	for (auto itr = m_texture_data3D_list.begin();
		itr != m_texture_data3D_list.end();
		itr++
		) {

		for (auto t : itr->second->texture_list) {

			t->Release();
			itr->second.reset();
		}
	}

	m_texture_data3D_list.clear();
}


void TextureManager::ReleaseTexture(const std::string &texture_name) {

	// テクスチャが存在しないなら
	if (FindTexture(texture_name) == false) {
		return;
	}

	// 解放
	m_texture_data2D_list[texture_name]->p_texture_buffer->Release();

	// TextureDataのメモリ解放
	m_texture_data2D_list[texture_name].reset();

	// 要素解放
	m_texture_data2D_list.erase(texture_name);
}


void TextureManager::ReleaseTextureMultiple(const std::string &texture_name) {

	// テクスチャが存在しないなら
	if (FindMultipleTexture(texture_name) == false) {
		return;
	}

	// 解放
	for (auto &itr : m_texture_data3D_list[texture_name]->texture_list) {
		itr->Release();
	}

	// TextureDataのメモリ解放
	m_texture_data3D_list[texture_name].reset();

	// 要素解放
	m_texture_data3D_list.erase(texture_name);
}