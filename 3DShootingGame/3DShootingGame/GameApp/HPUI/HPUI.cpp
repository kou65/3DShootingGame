﻿#include"HPUI.h"
#include"../../Lib/Sprite2D/Sprite2D/Sprite2DUser.h"
#include<math.h>


HPUI::HPUI(Player*p) {

	m_p_player = p;

	m_sprite_param.is_uv_slider = true;

	m_sprite_param.max_uv_slider.x = 1.f;
	m_sprite_param.max_uv_slider.y = 1.f;
	m_sprite_param.min_uv_slider.x = 0.f;
	m_sprite_param.min_uv_slider.y = 0.f;
}


void HPUI::Update() {

	// uv値計算

	// 最大hp受け取り
	float max_hp = Player::MAX_HP;

	// 現在のhp受け取り
	float hp = m_p_player->GetHp();

	// uv最大値から比率割り出し
	float i =  1.f / max_hp;

	// hp比率割り出し
	float current_uv = max_hp - hp;

	// uv変換
	current_uv *= i;
	
	m_sprite_param.max_uv_slider.x = (1.f - current_uv);
}


void HPUI::Draw() {


	m_sprite_param.texture_name = "hp_gauge";
	m_sprite_param.x = 1500.f;
	m_sprite_param.y = 50.f;

	Sprite2DUser::GetInstance()->BoardDraw(m_sprite_param);
}