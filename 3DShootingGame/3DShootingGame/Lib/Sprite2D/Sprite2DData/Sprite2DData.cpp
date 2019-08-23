#include"../Sprite2DData/Sprite2DData.h"




Sprite2DData::Sprite2DData(
	float x,
	float y,
	const char* texture_name,
	float scale_width,
	float scale_height,
	float angle,
	float ofset_x,
	float ofset_y,
	bool is_graph_uv_cut,
	int tu_cut_num,
	int tv_cut_num,
	int graph_num,
	float tu,
	float tv,
	Graphics::SamplerStateType state_type
) {

	// それぞれのプロパティに代入
	this->texture_name = texture_name;
	this->x = x;
	this->y = y;
	this->scale_width = scale_width;
	this->scale_height = scale_height;
	this->angle = angle;
	this->ofset_x = ofset_x;
	this->ofset_y = ofset_y;
	this->is_graph_uv_cut = is_graph_uv_cut;
	this->tu_cut_num = tu_cut_num;
	this->tv_cut_num = tv_cut_num;
	this->graph_num = graph_num;
	this->tu = tu;
	this->tv = tv;
	this->type = state_type;
}


