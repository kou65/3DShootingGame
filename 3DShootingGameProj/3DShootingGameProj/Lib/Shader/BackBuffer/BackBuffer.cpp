#include"BackBuffer.h"



BackBuffer::BackBuffer() {

	mp_device =
	Graphics::GetInstance()->GetDevice();
}


BackBuffer::~BackBuffer() {

	//mp_device_buffer->Release();
	//mp_device_depth_buffer->Release();
}


void BackBuffer::SaveBuffer(
	const int &target_index
) {

	// レンダリングする前に保存
	mp_device->GetRenderTarget(target_index,&mp_device_buffer);
	mp_device->GetDepthStencilSurface(&mp_device_depth_buffer);
	mp_device->GetViewport(&m_view_port);
}


void BackBuffer::SetBuffer(
const int&target_index
) {

	// レンダリングした後に保存したものを戻す
	mp_device->SetRenderTarget(target_index, mp_device_buffer);
	mp_device->SetDepthStencilSurface(mp_device_depth_buffer);
	//mp_device->SetViewport(&m_view_port);


	// 初期化
	mp_device_buffer = NULL;
	mp_device_depth_buffer = NULL;

	// GetSurfaceLevelで内部カウンタが+1されているのでReleaseして内部カウンタを減らす
	mp_device->SetVertexShader(NULL);
	mp_device->SetPixelShader(NULL);
}