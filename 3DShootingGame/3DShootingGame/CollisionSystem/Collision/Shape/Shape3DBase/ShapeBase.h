#pragma once
#include"../ShapeData/ShapeData.h"
#include"../../../../Lib/Vec3/Vec3.h"



class Shape3DBase {
public:

	Shape3DBase(const Vec3 &vec);

	Shape3DBase(
		const float&x,
		const float&y,
		const float&z
	);

	ShapeType GetShapeType();
	Vec3 GetPos();

protected:

	Vec3 m_vec;
	ShapeType m_shape_type;
};