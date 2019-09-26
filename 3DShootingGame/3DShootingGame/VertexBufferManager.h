#pragma once
#include<string>
#include<vector>
#include<unordered_map>
#include"Lib\Graphics\Graphics.h"



class VertexBufferManager {
public:

	static VertexBufferManager *GetInstance() {
		static VertexBufferManager vertex_buffer_manager;
		return &vertex_buffer_manager;
	}
public:

	bool Load(std::string vertex_buffer_name,IDirect3DVertexBuffer9*buffer);

private:

	std::unordered_map<std::string,IDirect3DVertexBuffer9*>m_vertex_buffer_list;
};