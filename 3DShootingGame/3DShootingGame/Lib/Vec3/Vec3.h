#pragma once
#include"../Window/Window.h"



struct Vec3 : public D3DXVECTOR3{

	Vec3() {
		x = 0.f;
		y = 0.f;
		z = 0.f;
	}

	Vec3(float x,float y,float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
};