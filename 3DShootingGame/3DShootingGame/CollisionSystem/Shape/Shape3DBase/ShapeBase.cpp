#include"ShapeBase.h"



Shape3DBase::Shape3DBase(const Vec3 &pos) {
	vec = pos;
}


Shape3DBase::Shape3DBase(
	const float&x,
	const float&y,
	const float&z
) {
	vec.x = x;
	vec.y = y;
	vec.z = z;
}


Vec3 Shape3DBase::GetPos() {
	return vec;
}


ShapeType Shape3DBase::GetShapeType() {
	return m_shape_type;
}