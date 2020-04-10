#include"Object3D.h"


Object3D::Object3D() : ObjectBase(){

	SetTag(Object3DTag::NONE);
}


Vec3 Object3D::GetPos() {
	return m_pos;
}


Object3DTag Object3D::GetTag() {
	return m_object_tag;
}


void Object3D::SetTag(const Object3DTag&tag) {
	m_object_tag = tag;
}