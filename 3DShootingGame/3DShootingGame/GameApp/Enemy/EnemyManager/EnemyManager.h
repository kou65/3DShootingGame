#pragma once
#include"../EnemyBase/EnemyBase.h"
#include<vector>



class EnemyManager {
public:

	EnemyManager() {

	}

	void SerchCreateDeleteTaile() {


	}

private:

	std::vector<EnemyBase*>m_p_enemy_list;
	CharacterBase*m_p_chara;
};