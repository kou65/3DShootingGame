#pragma once
#include"../../Shape/Shape3DBase/ShapeBase.h"



struct Sphere : public Shape3DBase{

	Sphere() {};

	// 半径
	float GetRadian();

	// 半径
	float radian;
};