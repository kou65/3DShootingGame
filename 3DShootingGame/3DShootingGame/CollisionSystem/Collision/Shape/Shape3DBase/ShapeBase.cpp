#include"ShapeBase.h"


Shape3DBase::Shape3DBase(const Vec3 &pos) {
	m_vec = pos;
}


Shape3DBase::Shape3DBase(
	const float&x,
	const float&y,
	const float&z
) {
	m_vec.x = x;
	m_vec.y = y;
	m_vec.z = z;
}


Vec3 Shape3DBase::GetPos() {
	return m_vec;
}


ShapeType Shape3DBase::GetShapeType() {
	return m_shape_type;
}