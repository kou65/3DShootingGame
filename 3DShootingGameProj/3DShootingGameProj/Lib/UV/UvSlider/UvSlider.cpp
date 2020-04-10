#include"UvSlider.h"



// スライダーの値 / 最大値 = 比率
// 最大値-10 最小値10 スライダーの値2

// 2 / 10 = 5;
// 2 - (-10)


Vec2 UvSlider::CalcGraphSizeSetUv(
	const Vec2 &current_max_uv,
	const Vec2 &current_min_uv,
	const Vec2 &max_graph_size
) {

	// 現在の最大画像数からuv値による現在画像数を割り出す
	// 現在のuv比率 = 画像サイズ * uvサイズ

	// 最大画像サイズ
	float max_graph_size_x = 
		max_graph_size.x * current_max_uv.x;
	float max_graph_size_y =
		max_graph_size.y * current_max_uv.y;

	// 最小画像サイズ
	float min_graph_size_x =
		max_graph_size.x * current_min_uv.x;
	float min_graph_size_y =
		max_graph_size.y * current_min_uv.y;

	// 変換済み画像サイズ
	Vec2 conv_graph_size;

	// 最大から最小を引き現在のuvにあった画像サイズにする
	conv_graph_size.x = max_graph_size_x - min_graph_size_x;
	conv_graph_size.y = max_graph_size_y - min_graph_size_y;


	// uv矩形に現在のuv代入
	SetMaxMinUvParam(
		current_max_uv,
		current_min_uv
	);

	return conv_graph_size;
}

void UvSlider::CalcUvByGraphSize(
	const Vec2&current_max_graph_size,
	const Vec2&current_min_graph_size,
	const Vec2&max_graph_size
) {

	// 現在uv数
	// 現在のuv比率 = 現在の画像サイズ / 最大画像サイズ

	Vec2 current_max_uv_size;
	Vec2 current_min_uv_size;

	// 最大uv算出
	current_max_uv_size.x =
		current_max_graph_size.x / max_graph_size.x;

	current_max_uv_size.y =
		current_max_graph_size.y / max_graph_size.y;

	// 最小uv算出
	current_min_uv_size.x =
		current_min_graph_size.x / max_graph_size.x;

	current_min_uv_size.y =
		current_min_graph_size.y / max_graph_size.y;

	// uvセット
	SetMaxMinUvParam(
		current_max_uv_size,
		current_min_uv_size
	);
}
