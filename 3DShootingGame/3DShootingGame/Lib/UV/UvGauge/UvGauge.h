#pragma once
#include"../UvBase/UvBase.h"



class UvGauge : public UvBase{
public:

	// 方向
	enum Direction {
		RIGHT,
		LEFT,
		TOP,
		BOTTOM,
	};

public:

	void WidthPlay(
		const int &graph_width_size,
	const Direction&dir
	);
};