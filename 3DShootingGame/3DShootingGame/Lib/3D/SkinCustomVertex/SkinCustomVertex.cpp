#include"SkinCustomVertex.h"



SkinCustomVertex::SkinCustomVertex() {


	vertex.x = vertex.y = vertex.z = 0.f;
	normal.x = normal.y = normal.z = 0.f;
	uv.x = uv.y = 0.f;

	// 初期値
	D3DXCOLOR col;
	col.a = 255.f;
	col.b = 255.f;
	col.g = 255.f;
	col.r = 255.f;

	diffuse = 0xfffffff;

}