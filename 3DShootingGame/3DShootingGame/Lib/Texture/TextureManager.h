#pragma once
#include"TextureData2D.h"
#include"TextureData3D.h"
#include<string>
#include<unordered_map>
#include<memory>



class TextureManager{
public:

	~TextureManager();
	
	static TextureManager *GetInstance() {

		static TextureManager texture_manager;
		return &texture_manager;
	}


	void Load2D(
		const char* file_name,
		const char* texture_rename,
		UINT width = 0,
		UINT height = 0,
		float tu = 0.f,
		float tv = 0.f
	);


	void Load3D(
		const char*file_name,
		const char*texture_name,
		const int &texture_num
	);


	bool Find2DTexture(std::string name);

	bool Find3DTexture(std::string name);

	// 2D用のテクスチャデータを返す
	TextureData2D &GetTextureData2D(std::string name);

	// 3D用のテクスチャデータを返す
	TextureData3D &GetTextureData3D(
		std::string name);

	// 解放
	void Release();


private:

	// 隠しコンストラクタ
	TextureManager() {};

	// 2Dのテクスチャリスト
	std::unordered_map<
		// キー
		std::string,
		// 入れ物
		TextureData2D*
	> m_texture_data2D_list;

	// 3Dのテクスチャリスト
	std::unordered_map<
		// キー
		std::string,
		// 入れ物
		TextureData3D*
	> m_texture_data3D_list;

};