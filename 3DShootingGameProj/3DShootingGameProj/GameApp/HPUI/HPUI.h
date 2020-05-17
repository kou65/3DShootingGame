
#ifndef HPUI_H
#define HPUI_H
#include"../../ObjectSystem/Object2D/Object2D.h"
#include"../Player/Player.h"
#include"../../Lib/Sprite2D/Sprite2DParameter/Sprite2DParameter.h"



/**
* @brief HPUIクラス
*/
class HPUI :public Object2D{
public:


	/**
	* @brief コンストラクタ
	* @param[in] p_player プレイヤーポインタ
	*/
	HPUI(Player*p_player);


	/**
	* @brief 更新 仮想関数
	*/
	void Update()override;


	/**
	* @brief 描画 仮想関数
	*/
	void Draw()override;

private:

	float GetCharaHp(CharacterInterface&chara_interface);

private:

	//! 2Dスプライト情報構造体
	Sprite2DParameter m_sprite_param;

	//! プレイヤーポインタ
	Player*mp_player;
};

#endif