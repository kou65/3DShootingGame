#pragma once
#include"../ZTextureManager/ZTextureManager.h"


/**
* @brief Zテクスチャの読み込み
*/
namespace FuncZTexture {


	/**
	* @brief 定数
	*/
	namespace Const{
		
		//! zテクスチャ
		const std::string Z_TEX_1024 = 
			"z_tex_1024";
	}

	/**
	* @brief 読み込み
	*/
	void Create();
}