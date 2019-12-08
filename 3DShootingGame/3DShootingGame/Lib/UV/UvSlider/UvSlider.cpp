#include"UvSlider.h"



void UvSlider::CalcRatio() {

	// 比率計算 (スライダーの値 - 最小値) / (最大値 - 最小値)
	s_param.ratio = (float)
		(s_param.slider_num - s_param.min_value) /
		(s_param.max_value - s_param.min_value);
}


void UvSlider::UpdateGraphSize(
	float &width_size,
	float &height_size
) {

	//width_size * s_param.ratio;
	//height_size * s_param.ratio;
}