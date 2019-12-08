#pragma once
#include"../UvRect/UvRect.h"


struct SliderParam {
	int slider_num;
	int max_value;
	int min_value;
	float ratio;
};


class UvSlider : public UvRect{
public:

	// 増減方向
	enum class Direction {
		UP,
		DOWN,
		RIGHT,
		LEFT
	};

public:

	UvSlider() {};

	void CalcRatio();

	void UpdateGraphSize(
		float &width_size,
		float &height_size
	);

private:

	// 比率計算用
	SliderParam s_param;
};