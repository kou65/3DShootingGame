#include"AnimationParameter.h"


AnimationParamter::AnimationParamter() :
	// 初期値
	division_width(0),
	division_height(0),
	current_graph_num(0),
	is_animation(false),
	animation_frame(0.f),
	current_animation_count(0),
	use_type(Mode::LOOP),
	turn_type(TurnType::RIGHT)
{


}