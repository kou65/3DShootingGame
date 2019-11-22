#pragma once
#include"../UvRect/UvRect.h"
#include"../../Vec2/Vec2.h"



struct UvRect {

	// テクスチャ頂点座標
	Vec2 m_top_left_pos;
	Vec2 m_top_right_pos;
	Vec2 m_bottom_left_pos;
	Vec2 m_bottom_right_pos;
};
