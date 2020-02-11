#pragma once
#include<vector>
#include<string>


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

}