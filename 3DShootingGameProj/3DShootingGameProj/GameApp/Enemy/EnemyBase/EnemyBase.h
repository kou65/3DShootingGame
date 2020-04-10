#pragma once
#include"../../CharacterBase/CharacterBase.h"



class EnemyBase : public CharacterBase{
public:

	void Destory() {
		m_is_active = false;
	}

protected:

	float m_hp;
};