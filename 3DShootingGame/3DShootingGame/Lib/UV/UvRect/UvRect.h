#pragma once
#include"../UvRect/UvRect.h"
#include"../../Vec2/Vec2.h"



struct UvRect {

	void operator +=(const UvRect &rect) {

		// 操作
		top_left_pos += rect.top_left_pos;
		top_right_pos += rect.top_right_pos;

		bottom_left_pos += rect.bottom_left_pos;
		bottom_right_pos += rect.bottom_right_pos;
	}

	// テクスチャ頂点座標
	Vec2 top_left_pos;
	Vec2 top_right_pos;
	Vec2 bottom_left_pos;
	Vec2 bottom_right_pos;
};
