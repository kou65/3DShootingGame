#include"Utility.h"
#include<stdio.h>
#include<string>
#include"../Vec3/Vec3.h"



namespace Utility {

	namespace Convert {

		std::vector<std::string> SplitStr(
			char cut_base_str,
			const std::string &string
		) {

			std::string string_buffer;
			const char*string_ = string.c_str();

			std::vector<std::string> out_string_list;


			// 文字列が存在しないなら
			if (string.c_str() == NULL) {
				return out_string_list;
			}

			for (unsigned int i = 0; i < strlen(string.c_str()); i++) {

				// 文字列加算
				string_buffer += string_[i];

				// 区切りを指定
				if (cut_base_str == string_[i]) {

					std::string str(string_buffer);

					// 文字列配列に追加
					out_string_list.push_back(str);

					string_buffer.clear();
				}
			}

			// 最後の文字列算出
			{

				std::string str(string_buffer);

				out_string_list.push_back(str);
			}

			return out_string_list;
		}


		void SplitStr(
			char *m_root_path,
			const char*file_path,
			char split_str = '/'
		) {

			int str_num = (int)strlen(file_path);
			str_num *= 2;

			strcpy_s(
				m_root_path,
				str_num,
				file_path
			);

			size_t i;
			for (i = strlen(m_root_path); 0 < i; i--)
			{
				if (m_root_path[i] == split_str) break;
			}
			m_root_path[i] = '\0';

		}

		bool IsStrCmp(
			const char*str_cmp1,
			const char*str_cmp2
		) {

			int result = strcmp(str_cmp1, str_cmp2);

			// 0なら文字列が一致
			if (result == 0) {
				return true;
			}
			else {
				return false;
			}
		}

		bool IsStrInclude(
			const char*str1,
			const char*str2
		) {

			if (strstr(str1, str2) == NULL) {
				return false;
			}

			return true;
		}


		std::string ValueToString(int num) {
			return std::to_string(num);
		}

		int StringToValue(const std::string &str) {
			return atoi(str.c_str());
		}

		int StringToValue(const char*str) {
			return atoi(str);
		}

	}

	namespace File {

		bool Open(
			FILE**p_fp,
			const std::string&load_file_name,
			const char*mode
		) {

			// ファイルを開く
			fopen_s(&*p_fp, load_file_name.c_str(), mode);

			if (p_fp == NULL) {
				return false;
			}

			return true;
		}


		void Close(FILE**p_file) {

			if (p_file == nullptr) {
				return;
			}

			// ファイルを閉じる
			fclose(*p_file);
		}


		void LoadFloatVec3(FILE*p_fp, Vec3&out_vec3, char break_str) {

			std::string str;

			// 3つの%fを作る
			for (int i = 0; i < 2; i++) {
				str += "%f";
				str += break_str;
			}
			// 最後の%f
			str += "%f";

			// x軸,y軸,z軸を取得
			fscanf_s(
				p_fp, str.c_str(),
				&out_vec3.x,
				&out_vec3.y,
				&out_vec3.z);
		}


		bool LoadIntNum(
			FILE*p_file,
			std::vector<int>&out_num_list,
			const int&load_total_num,
			const char *break_str
		) {

			std::string str;

			// 文字列連結
			str += "%d";
			str += break_str;

			int num = -1;
			bool is_load = true;
			int ret_num = -1;

			for (int i = 0; i < load_total_num; i++) {

				// 値(intかfloatか)を取得
				ret_num = fscanf_s(
					p_file,
					str.c_str(),
					&num
				);

				// 加える
				out_num_list.push_back(num);
			}

			// 正式に読み込めてなかったら
			if (ret_num == 0) {
				is_load = false;
			}

			return is_load;
		}
	}

}