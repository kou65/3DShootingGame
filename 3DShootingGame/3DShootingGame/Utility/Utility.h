#pragma once
#include<vector>
#include<string>
#include"../Lib/Vec3/Vec3.h"



namespace Utility {


	std::vector<std::string> SplitStr(
		char cut_base_str,
		const std::string &string
	);

	void SplitStr(
		char *m_root_path,
		const char*file_path,
		char split_str
	);

	std::string ValueToString(int num);
	int StringToValue(const std::string &str);
	int StringToValue(const char*str);


	bool IsStrCmp(
		const char*str_cmp1,
		const char*str_cmp2
	);

	bool IsStrInclude(
		const char*str1,
		const char*str2
	);

	// ファイルを開く
	bool FileOpen(
		FILE**p_fp,
		const std::string&load_file_name,
		const char*mode
	);

	// ファイルを閉じる
	void FileClose(FILE**p_file);


	void LoadFloatVec3(FILE*p_file,Vec3&v,char break_str);


}