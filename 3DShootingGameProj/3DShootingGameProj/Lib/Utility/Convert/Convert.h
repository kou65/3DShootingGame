#pragma once

#include<vector>
#include<string>
#include<iostream>



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
}