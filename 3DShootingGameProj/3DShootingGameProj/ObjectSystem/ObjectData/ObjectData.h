﻿
#ifndef OBJECT_DATA_H
#define OBJECT_DATA_H

#include"../../GameApp/Player/Player.h"
#include"../../GameApp/GoalObject/GoalObject.h"
#include<memory>



/**
* @brief 情報をまとめて置いておく用のObjectData構造体
*/
struct ObjectData {

	
	//! playerポインタ
	std::weak_ptr<Player> p_player;

	//! ゴールオブジェクトポインタ
	std::weak_ptr<GoalObject>p_goal_object;

};


#endif