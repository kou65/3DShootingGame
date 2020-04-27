
#ifndef CHARA_INTERFACE_H
#define CHARA_INTERFACE_H
#include"../CharacterBase/CharacterBase.h"



/**
* @brief 基底キャラクタを隠蔽化して相手のクラスに開示する用のクラス
*/
class CharacterInterface {
public:


	/**
	* @brief デフォルトコンストラクタ
	*/
	CharacterInterface() {}


	/**
	* @brief hpをセット
	* @param[in] 説明同上
	*/
	void SetHp(const float &hp) {
		m_hp = hp;
	}


	/**
	* @brief hpを返す
	* @return float 説明同上
	*/
	float GetHp() {
		return m_hp;
	}

private:

	//! 保持用hp
	float m_hp;
};

#endif