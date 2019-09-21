#pragma once
#include"Lib\Graphics\Graphics.h"


struct Vec3{

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

	float x;
	float y;
	float z;
};