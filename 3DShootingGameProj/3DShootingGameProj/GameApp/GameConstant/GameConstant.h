#ifndef GAME_CONSTANT_H
#define GAME_CONSTANT_H

#include<string>



/**
* @brief ゲームアプリに使う定数
*/
namespace Const {


	/**
	* @brief 画像に使う定数
	*/
	namespace Graph {
		const std::string TAILE = "taile";
		const std::string BACK_GROUND = "back_ground";
	}


	/**
	* @brief Objファイルに登録したobjを呼び出す時に使う定数
	*/
	namespace Obj {

		const std::string PLAYER = "player";
		const std::string CUBE = "enemy";
		const std::string SPEHER = "bullet";
		const std::string ENEMY_BULLETER = "enemy_bulleter";
		const std::string PLANE = "plane";
	}


	/**
	* @brief ファイルから読み込んだ時に使う識別定数
	*/
	namespace FileObj {

		const std::string PLAYER = "res1";
		const std::string BULLET_ENEMY = "res2";
		const std::string BACK_GROUND = "res3";
		const std::string CUBE = "res4";
		const std::string SHOTGUN_ENEMY = "res5";
		const std::string GOAL_OBJECT = "res6";
	}

}


#endif