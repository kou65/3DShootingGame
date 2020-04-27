#pragma once
#include"../../../Lib/Vec3/Vec3.h"


/**
* @enum MapTag マップのナンバー
* @brief 必ずテキストに書かれている番号順にする
*/
enum class MapTag {
	NONE,
	BLOCK,
	ENEMY1,
	ENEMY2
};


/**
* @brief マップデータ
*/
struct MapData {

	Vec3 pos;
};