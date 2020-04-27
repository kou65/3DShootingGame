#pragma once
#include"../../CharacterBase/CharacterBase.h"



/**
* @brief 敵基底クラス
*/
class EnemyBase : public CharacterBase{
public:

	/**
	* @brief 削除関数
	*/
	void Destory() {
		m_is_active = false;
	}
};