#pragma once
#include<string>
#include<unordered_map>
#include"Lib\Graphics\Graphics.h"


class IndexBufferManager {
public:

	// アクセス権
	static IndexBufferManager *GetInstance() {
		static IndexBufferManager index_buffer_manager;
		return &index_buffer_manager;
	}
public:

	bool Load(const std::string &index_buffer_name, IDirect3DIndexBuffer9*buffer);

private:

	std::unordered_map<std::string, IDirect3DIndexBuffer9*>m_index_buffer_list;
};