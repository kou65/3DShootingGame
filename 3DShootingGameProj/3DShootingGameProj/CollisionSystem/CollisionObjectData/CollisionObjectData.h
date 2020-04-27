#pragma once


/**
* @brief 衝突オブジェクトの種類
*/
enum class CollisionObjectType {
	NONE,
	PLAYER,
	ENEMY,
	PLAYER_BULLET,
	ENEMY_BULLET,
	BREAK_BULLET,
	BLOCK,
	TOTAL,
};
