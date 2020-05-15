
#ifndef UTILITY_H
#define UTILITY_H

#include<vector>
#include<string>
#include<random>
#include<iostream>
#include"../Vec3/Vec3.h"
#include"../Math/Math.h"



/**
* @brief プログラムを関係で実用なものが入っている 
*/
namespace Utility {


	/**
	* @brief 変換まとめ
	*/
	namespace Convert {


		/**
		* @brief 文字分割
		*/
		std::vector<std::string> SplitStr(
			char cut_base_str,
			const std::string &string
		);


		/**
		* @brief 文字分割
		*/
		void SplitStr(
			char *m_root_path,
			const char*file_path,
			char split_str
		);


		/**
		* @brief 数字から文字列へ変換
		*/
		std::string ValueToString(int num);


		/**
		* @brief 文字列から数字へ変換
		*/
		int StringToValue(const std::string &str);


		/**
		* @brief 文字列から数字へ変換
		*/
		int StringToValue(const char*str);


		/**
		* @brief 文字列連結関数
		*/
		bool IsStrCmp(
			const char*str_cmp1,
			const char*str_cmp2
		);


		/**
		* @brief 文字列を読み込んでるかどうか
		*/
		bool IsStrInclude(
			const char*str1,
			const char*str2
		);
	}


	/**
	* @brief ファイルまとめ
	*/
	namespace File{
		/**
		* @brief ファイルを開く
		*/
		bool Open(
			FILE**p_fp,
			const std::string&load_file_name,
			const char*mode
		);


	/**
	* @brief ファイルを閉じる
	*/
	void Close(FILE**p_file);


	/**
	* @brief ファイルからFLOAT3の値を取ってくる
	*/
	void LoadFloatVec3(FILE*p_file, Vec3&v, char break_str);


	/**
	* @brief INTの数字を読み込む
	* @param[in] p_file ファイルポインタ
	* @param[out] out_num 外部へ読み込んだ数字を出力
	* @param[in] load_total_num 1行に読み込む数字の数
	* @param[in] break_str 読み込んでいる時の区切り文字
	*/
	bool LoadIntNum(
		FILE*p_file,
		std::vector<int>&out_num_list,
		const int&load_total_num,
		const char *break_str = ""
	);
}

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
#endif