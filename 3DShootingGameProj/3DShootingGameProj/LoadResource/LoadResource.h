
#ifndef LOAD_RESORUCE_H
#define LOAD_RESORUCE_H
#include<string>



/**
* @brief 素材を読み込む
*/
namespace  LoadResources {


	/**
	* @brief 読み込んだファイル名
	*/
	namespace Name {

		//! クリアロゴ
		const std::string CLEAR_LOGO = "clear_logo";
	}


	/**
	* @brief リソース読み込み
	*/
	void Load();

}

#endif
