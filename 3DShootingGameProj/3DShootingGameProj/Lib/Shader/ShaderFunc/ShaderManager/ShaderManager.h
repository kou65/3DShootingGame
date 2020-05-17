#pragma once
#include"../ShaderBase/EffectFileBase.h"
#include<map>
#include<string>



/**
* @brief シェーダーファイルをここにまとめて管理するクラス
*/
class ShaderManager {
public:


	/**
	* @brief シェーダークラス挿入
	*/
	void Entry(const char*register_file_name,ShaderBase*base) {
		mp_shader_list[register_file_name] = base;
	}


	/**
	* @brief 削除
	*/
	void Exit(
		const std::string&register_name
	) {};


private:

	//! 管理者
	std::map<std::string, ShaderBase*>mp_shader_list;
};