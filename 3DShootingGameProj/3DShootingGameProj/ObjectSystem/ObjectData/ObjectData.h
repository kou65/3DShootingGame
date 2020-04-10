#pragma once
#include"../../GameApp/Player/Player.h"
#include"../../GameApp/GoalObject/GoalObject.h"



// 情報をまとめて置いておく用のObjectData構造体

struct ObjectData {

	ObjectData(){
		p_player = nullptr;
		p_goal_object = nullptr;
	}

	Player * p_player;
	GoalObject*p_goal_object;
};