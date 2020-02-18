#pragma once
#include"../../Shape/Shape3DBase/ShapeBase.h"



class Sphere : public Shape3DBase{
public:

	float GetRadian();

private:

	// 半径
	float m_radian;
};