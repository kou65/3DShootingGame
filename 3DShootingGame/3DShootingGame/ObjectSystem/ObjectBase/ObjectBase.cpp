#include"ObjectBase.h"


ObjectBase::ObjectBase() {
	m_is_active = true;
}


bool ObjectBase::IsActive() {
	return m_is_active;
}