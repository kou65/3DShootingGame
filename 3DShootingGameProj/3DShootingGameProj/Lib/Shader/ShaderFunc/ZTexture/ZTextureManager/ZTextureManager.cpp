#include"ZTextureManager.h"



void ZTextureManager::Update() {


	// メモリ解放
	for (auto&itr : m_z_tex_list) {
		itr.second->Update();
	}
}


void ZTextureManager::Create(
const std::string&register_name,
const Vec2 &tex_size
) {

	// 生成
	m_z_tex_list[register_name] = 
		new ZTexture();

	// テクスチャサイズセット
	m_z_tex_list[register_name]->
		SetTextureSize(tex_size.x,tex_size.y);

	// 初期化
	m_z_tex_list[register_name]->Init();

	m_z_tex_list[register_name]->Update();
}


void ZTextureManager::Destory(
	const std::string&register_name
) {

	// メモリ解放
	delete m_z_tex_list[register_name];

	// 要素削除
	m_z_tex_list.erase(register_name);
}


ZTexture* ZTextureManager::GetZTexturePtr(
	const std::string&register_name
) {
	return m_z_tex_list[register_name];
}


void ZTextureManager::AllDestory() {


	// メモリ解放
	for (auto&itr : m_z_tex_list) {
		delete itr.second;
	}

	// 要素の削除
	m_z_tex_list.clear();
}