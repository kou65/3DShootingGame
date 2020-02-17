#include"../Sprite3DParameter/Sprite3DParameter.h"



Sprite3DParameter::Sprite3DParameter(
	float pos_x,
	float pos_y,
	float pos_z,
	const char* texture_name,
	PolygonDirection dir,
	float scale_width,
	float scale_height,
	bool is_back_cull_mode,
	float ofset_x,
	float ofset_y,
	bool is_graph_uv_cut,
	int tu_cut_num,
	int tv_cut_num,
	int graph_num,
	float tu,
	float tv,
	D3DXCOLOR color
) {

	// 各プロパティ設定
	this->pos.x = pos_x;
	this->pos.y = pos_y;
	this->pos.z = pos_z;
	this->texture_name = texture_name;
	this->scale_width = scale_width;
	this->scale_height = scale_height;
	this->polygon_dir = dir;
	this->is_back_cull_mode = is_back_cull_mode;
	this->is_graph_uv_cut = is_graph_uv_cut;
	this->ofset.x = ofset_x;
	this->ofset.y = ofset_y;
	this->tu_cut_num = tu_cut_num;
	this->tv_cut_num = tv_cut_num;
	this->graph_num = graph_num;
	this->tu = tu;
	this->tv = tv;
	this->color = color;
}