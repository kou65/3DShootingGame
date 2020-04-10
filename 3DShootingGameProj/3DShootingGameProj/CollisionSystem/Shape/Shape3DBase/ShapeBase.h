#pragma once
#include"../ShapeData/ShapeData.h"
#include"../../../Lib/Vec3/Vec3.h"



struct Shape3DBase {

	Shape3DBase(const Vec3 &vec);

	Shape3DBase(
		const float&x,
		const float&y,
		const float&z
	);

	Shape3DBase() {};

	ShapeType GetShapeType();
	Vec3 GetPos();

	Vec3 vec;
	ShapeType m_shape_type;
};