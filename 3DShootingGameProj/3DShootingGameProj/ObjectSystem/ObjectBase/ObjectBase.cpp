#include"ObjectBase.h"


ObjectBase::ObjectBase() {
	m_is_active = true;
	m_can_draw = true;
}


bool ObjectBase::IsActive() {
	return m_is_active;
}


bool ObjectBase::CanDraw() {
	return m_can_draw;
}


void ObjectBase::Exit() {
	m_is_active = false;
}