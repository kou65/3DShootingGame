#include"Utility.h"
#include<stdio.h>
#include<string>



namespace Utility {

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


	std::string ValueToString(int num) {
		return std::to_string(num);
	}

	int StringToValue(const std::string &str) {
		return atoi(str.c_str());
	}

	int StringToValue(const char*str) {
		return atoi(str);
	}


	FILE* FileOpen(
		const std::string&load_file_name,
		const char*mode
	) {

		FILE * fp;

		// ファイルを開く
		fopen_s(&fp, load_file_name.c_str(), mode);

		if (fp == NULL) {
			return fp;
		}
	}


	void FileClose(FILE*p_file) {

		// ファイルを閉じる
		fclose(p_file);
	}



	//void LoadUntilLineFeed(FILE*p_file,);

}