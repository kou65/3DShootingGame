#include"../Sprite2DParameter/Sprite2DParameter.h"




Sprite2DParameter::Sprite2DParameter(
	float x,
	float y,
	const char* texture_name,
	float scale_width,
	float scale_height,
	float angle,
	float ofset_x,
	float ofset_y,
	float tu,
	float tv,
	DWORD color,
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
	this->tu = tu;
	this->tv = tv;
	this->color = color;
	this->type = state_type;
}


