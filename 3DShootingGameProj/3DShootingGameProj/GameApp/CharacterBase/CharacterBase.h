#pragma once
#include"../Filed/MapObjectBase/MapObjectBase.h"



/**
* @brief 基底キャラクタ
*/
class CharacterBase : public MapObjectBase{
public:


	/**
	* @brief 更新 仮想関数
	*/
	void Update()override {};


	/**
	* @brief 描画 仮想関数
	*/
	void Draw()override {};

protected:

	
	/**
	* @brief 死亡
	*/
	void Death();


	/**
	* @brief ダメージ加算
	*/
	void AddDamage(const float&damage);

protected:
	
	//! 体力
	float m_hp;
};