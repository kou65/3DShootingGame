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
	Graphics::SamplerStateType state_type
) {

	// それぞれのプロパティに代入
	this->texture_name = texture_name;
	this->pos.x = x;
	this->pos.y = y;
	this->scale.x = scale_width;
	this->scale.y = scale_height;
	this->angle = angle;
	this->offset.x = ofset_x;
	this->offset.y = ofset_y;
	this->uv.x = tu;
	this->uv.y = tv;
	this->type = state_type;
	this->p_tex = nullptr;
	this->is_uv_slider = false;
	this->tex_size.x = 0.f;
	this->tex_size.y = 0.f;
	this->max_uv_slider.Initialization();
	this->min_uv_slider.Initialization();


	// カラーは白
	this->color = Vec3(255.f, 255.f, 255.f);
	// 透過度0
	this->alpha_value = 255;

}


