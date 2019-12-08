#pragma once
#include"../Window/Window.h"



struct Vec2 :public D3DXVECTOR2{

	Vec2(
		const float &x,
		const float &y
	) {
		this->x = x;
		this->y = y;
	}

	Vec2() {
		Initialization();
	};

	void Initialization() {
		x = 0.f;
		y = 0.f;
	}
};