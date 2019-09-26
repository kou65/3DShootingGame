#include"VertexBufferManager.h"


bool VertexBufferManager::Load(std::string vertex_buffer_name, IDirect3DVertexBuffer9*buffer) {

	m_vertex_buffer_list[vertex_buffer_name] = buffer;

	return true;
}