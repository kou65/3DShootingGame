#pragma once
#include"../ZTexture.h"
#include"../../DepthShadowShader/DepthShadowEffectFile.h"
#include"../../../../Vec2/Vec2.h"
#include<memory>
#include<map>



/**
* @brief Zテクスチャ管理シングルトンクラス
*/
class ZTextureManager {
public:

	/**
	* @brief アクセス権
	*/
	static ZTextureManager*GetInstance() {
		static ZTextureManager z_tex;
		return &z_tex;
	}

public:

	/**
	* @brief 全てのZテクスチャ更新
	*/
	void Update();


	/**
	* @brief 生成
	* @param[in] 登録名
	* @param[in] テクスチャサイズ
	*/
	void Create(
		const std::string&register_name,
		const Vec2 &tex_size
	);


	/**
	* @brief 削除
	*/
	void Destory(
		const std::string&register_name
	);


	/**
	* @brief Zテクスチャのポインタを返す 
	*/
	ZTexture *GetZTexturePtr(
		const std::string&register_name
	);


	/**
	* @brief 全て削除
	*/
	void AllDestory();

private:


	/**
	* @brief コンストラクタ
	*/
	ZTextureManager() {};

private:


	//! Zテクスチャまとめ
	std::map<std::string,ZTexture*>
		m_z_tex_list;
};