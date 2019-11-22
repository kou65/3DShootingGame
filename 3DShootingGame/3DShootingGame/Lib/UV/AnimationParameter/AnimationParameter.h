#pragma once



// アニメーションを回す方向
enum class TurnType {
	RIGHT,
	LEFT
};

// どう使用するか
enum class Mode {
	LOOP,             // ループ
	ONLY_ONCE         // 一回だけ
};

struct AnimationParamter {

	AnimationParamter();

	int division_width;
	int division_height;
	int current_graph_num;
	bool is_animation;
	int current_animation_count;
	float animation_frame;
	Mode use_type;
	TurnType turn_type;
};