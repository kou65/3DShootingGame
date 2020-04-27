
#ifndef OBJECT_DATA_H
#define OBJECT_DATA_H

#include"../../GameApp/Player/Player.h"
#include"../../GameApp/GoalObject/GoalObject.h"
#include<memory>



/**
* @brief 情報をまとめて置いておく用のObjectData構造体
*/
struct ObjectData {

	/**
	* @brief コンストラクタ
	*/
	ObjectData(){
		p_player = nullptr;
		p_goal_object = nullptr;
	}
	
	//! playerポインタ
	Player * p_player;

	//! ゴールオブジェクトポインタ
	GoalObject*p_goal_object;

};


struct ObjectData2 {

	std::weak_ptr<Player>p_player;
	std::weak_ptr<GoalObject>p_goal_object;
};
#endif