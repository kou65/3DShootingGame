#pragma once
#include"../ShaderBase/EffectFileBase.h"
#include<unordered_map>
#include<string>



class ShaderManager {
public:

	void Entry(const char*register_file_name,ShaderBase*base) {
		m_p_shader_list[register_file_name] = base;
	}

private:

	// 管理者
	std::unordered_map<std::string, ShaderBase*>m_p_shader_list;
};