#pragma once

#include<d3dx9.h>
#include"../../Vec2/Vec2.h"



/**
* @brief レンダーターゲットデータ
*/
struct RenderTargetData {

	RenderTargetData() {

		width = 1024;
		height = 1024;
		view_port_pos = Vec2(1920.f,1080.f);
		view_port_size_w = 100;
		view_port_size_h = 100;
		
	}

	DWORD width;
	DWORD height;
	Vec2 view_port_pos;
	DWORD view_port_size_w;
	DWORD view_port_size_h;
};