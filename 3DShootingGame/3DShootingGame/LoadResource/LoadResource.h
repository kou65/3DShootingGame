#pragma once
#include"../Lib/Texture/TextureManager.h"
#include<string>


namespace  Resource {

	// 読み込んだファイル名
	namespace Name {

		// クリアロゴ
		const std::string CLEAR_LOGO = "clear_logo";
	}

	// リソース読み込み
	void LoadResource();

}
