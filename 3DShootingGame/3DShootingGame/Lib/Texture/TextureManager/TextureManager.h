#pragma once
#include"../TextureData2D/TextureData2D.h"
#include"../TextureData3D/TextureData3D.h"
#include<string>
#include<unordered_map>
#include<memory>



class TextureManager{
public:
	
	static TextureManager *GetInstance() {

		static TextureManager texture_manager;
		return &texture_manager;
	}

public:

	~TextureManager();

	/**
	* @テクスチャの読み込み
	*/
	void Load(
		const char* file_name,
		const char* texture_rename,
		UINT width = 0,
		UINT height = 0,
		float tu = 0.f,
		float tv = 0.f
	);


	/**
	* @brief 複数テクスチャ読み込み
	*/
	void LoadMultiple(
		const char*file_name,
		const char*texture_name,
		const int &texture_num
	);


	bool FindTexture(std::string name);

	bool FindMultipleTexture(std::string name);

	// 2D用のテクスチャデータを返す
	TextureData &GetTextureData(std::string name);

	// XFile用のテクスチャデータを返す
	TextureMultipleData &GetTextureDataMultiple(std::string name);

	// テクスチャ2Dを解放
	void ReleaseTexture(std::string &texture_name);

	// テクスチャ3Dを解放
	void ReleaseTextureMultiple(std::string &texture_name);

	// 解放
	void AllRelease();


private:

	// 隠しコンストラクタ
	TextureManager() {};

	// テクスチャリスト
	std::unordered_map<
		// キー
		std::string,
		// 入れ物
		TextureData*
	> m_texture_data2D_list;

	// 複数のテクスチャリスト
	std::unordered_map<
		// キー
		std::string,
		// 入れ物
		TextureMultipleData*
	>m_texture_data3D_list;
};