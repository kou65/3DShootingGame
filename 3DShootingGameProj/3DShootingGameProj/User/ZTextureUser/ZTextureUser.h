#pragma once
#include"../../Lib/Shader/ShaderFunc/ZTexture/ZTextureManager/ZTextureManager.h"
#include"../../Debugger/ZTexSpriteCreater.h"



/**
* @brief zテクスチャを使用する使用者
*/
class ZTextureUser {
public:


	/**
	* @brief コンストラクタ
	*/
	ZTextureUser();


	/**
	* @brief 初期化
	*/
	void Init();


	/**
	* @brief 更新
	*/
	void Update();


	/**
	* @brief 描画
	*/
	void Draw();

private:


	/**
	* @brief 影データの更新
	*/
	void UpdateShadowData();


private:
	// !zテクスチャの板描画
	ZTexSpriteCreater m_z_tex_sprite;

};