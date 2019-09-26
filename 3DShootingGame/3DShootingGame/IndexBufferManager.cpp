#include"IndexBufferManager.h"




bool IndexBufferManager::Load(const std::string &index_buffer_name,IDirect3DIndexBuffer9*buffer) {

	m_index_buffer_list[index_buffer_name] = buffer;

	return true;
}