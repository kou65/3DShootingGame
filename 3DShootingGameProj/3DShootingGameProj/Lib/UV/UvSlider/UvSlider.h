#pragma once
#include"../UvBase/UvBase.h"

// 画像サイズを 0 ~ 100
// 1の比率で測る 0.f 1.f

// 50なら0.5
// 0.5なら50

struct SliderParam {

	// 現在uvサイズ(0.0 ~ 1.0)
	Vec2 current_max_uv_size;
	Vec2 current_min_uv_size;

	// スライダー現在の最大画像サイズ
	Vec2 current_max_graph_size;
	// スライダー現在の最小画像サイズ
	Vec2 current_min_graph_size;

};


class UvSlider : public UvBase{
public:

	UvSlider() {};

	// uvから画像サイズを割り出す
	// 戻り値変換済み画像サイズ
	Vec2 CalcGraphSizeSetUv(
		const Vec2 &current_max_uv,
		const Vec2 &current_min_uv,
		const Vec2&max_graph_size
	);

	// 画像サイズからuvサイズを割り出す
	void CalcUvByGraphSize(
		const Vec2&current_max_graph_size,
		const Vec2&current_min_graph_size,
		const Vec2&max_graph_size
	);

private:

	// 比率計算用
	SliderParam m_s_param;

	// 変換後のサイズ
	Vec2 m_convert_max_uv;
	Vec2 m_convert_min_uv;

	Vec2 m_convert_graph_size;
};