#include"CharacterBase.h"




void CharacterBase::Death() {

	if (m_hp <= 0) {
		m_is_active = false;
	}
}


void CharacterBase::AddDamage(const float&damage) {
	m_hp -= damage;
}