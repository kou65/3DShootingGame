#pragma once
#include<iostream>
#include<vector>
#include"../../Vec3/Vec3.h"



namespace Utility {

	/**
	* @brief ファイルまとめ
	*/
	namespace File {
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

}