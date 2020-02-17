#pragma once
#include"../Vec2/Vec2.h"
#include"../Vec3/Vec3.h"


struct TransformData3D {

	Vec3 pos;
	Vec3 rota;
	Vec3 scale;
};


struct TransformData2D {
	Vec2 pos;
	Vec2 rota;
	Vec2 scale;
};