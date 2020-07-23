#pragma once

#include<vector>
#include<string>
#include<random>
#include<iostream>
#include<Windows.h>



namespace Utility {

	// テンプレートの実装はヘッダでないと
		// 未解決外部参照になる

		/**
		* @brief ランダムまとめ
		*/
	namespace Random {

		// 整数を返す
		// 省略時はUINT
		template<typename T = UINT>
		T GetIntRandom(T min, T max) {

			// デバイス
			std::random_device rd;
			// シード
			std::mt19937 engine(rd());
			// 整数値
			std::uniform_int_distribution<T>
				dist(min, max);

			return dist(engine);
		}


		// 実数を返す
		// 省略時はfloat
		template<typename T = float>
		T GetRealRandom(T min, T max) {

			std::random_device rd;
			std::mt19937 engine(rd());
			std::uniform_real_distribution<T> dist(min, max);
			return dist(engine);
		}
	}
}