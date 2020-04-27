

#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H

#include"../ObjectManager/ObjectManager.h"
#include"../../Lib/3D/Camera3D/Camera3D.h"
#include"../../GameApp/Filed/Taile/Taile.h"
#include"../../Lib/Vec3/Vec3.h"
#include"../../GameApp/CharacterBase/CharacterBase.h"
#include"../../GameApp/Filed/BackGround/BackGround.h"
#include"../../GameApp/Enemy/EnemyBase/EnemyBase.h"
#include"../../GameApp/Block/Block.h"
#include"../../GameApp/Bullet/BulletData/BulletData.h"


/**
* @brief 前方宣言
*/
class Player;
class GoalObject;
struct ObjectData;
struct ObjectData2;


/**
* @brief オブジェクトを生成する工場クラス
*/
class ObjectFactory {
public:


	
	/**
	* @brief 自機生成
	* @param[in] pos 位置
	* @param[in] p_camera カメラポインタ
	* @param[out] p_out_object_data  
	*/
	void CreatePlayer(
		const Vec3&pos,
		std::shared_ptr<Camera3D>p_camera,
		std::shared_ptr<ObjectData>p_out_object_data
	);


	// 自機共有用
	void CreateSharedPlayer(
		const Vec3&pos,
		std::weak_ptr<Camera3D>m_p_camera,
		std::weak_ptr<ObjectData2>p_player_data
	);


	/**
	* @brief hpui生成
	* @param[in] object_data オブジェクトデータポインタ
	*/
	void CreateHPUI(
		std::shared_ptr<ObjectData>object_data
	);


	/**
	* @brief 弾生成
	* @param[in] data 弾データ
	*/
	void CreateBullet(
		const ObjParameter&param,
		const BulletData&data
	);


	/**
	* @brief 敵の弾生成
	* @param[in] pos 位置
	* @param[in] speed 弾の速度
	* @param[in] distance 距離
	* @param[in] dir 方向
	*/
	void CreateEnemyBullet(
		const ObjParameter&param,
		const BulletData&data
	);


	/**
	* @brief 破壊弾生成
	* @param[in] data 弾データ
	*/
	void CreateBreakBullet(
		const ObjParameter &param,
		const BulletData&data
	);


	/**
	* @brief 敵生成
	* @param[in] pos 位置
	* @param[in] speed 速度
	*/
	void CreateEnemy(
		const Vec3 &pos,
		const float &speed
	);


	/**
	* @brief その方向へ弾を撃つ敵生成
	* @param[in] pos 位置
	* @param[in] p_chara キャラクターポインタ
	* @param[out] p_enemy_base 基底敵ダブルポインタ
	*/
	void CreateHEnemy(
		const Vec3&pos,
		CharacterBase*p_chara,
		EnemyBase**p_enemy_base
	);


	/**
	* @brief ショットガンを撃つ敵
	* @param[in] pos 位置
	* @param[in] p_chara キャラクター基底ポインタ
	* @param[out] p_enemy_base 基底敵ダブルポインタ
	*/
	void CreateShotgunEnemy(
		const Vec3&pos,
		CharacterBase*p_chara,
		EnemyBase**p_enemy_base
	);


	/**
	* @brief ブロック作成
	* @param[in] pos 位置
	* @param[out] p_out_cube ブロックポインタ
	*/
	void CreateCube(
		const Vec3&pos,
		Block**p_out_cube
	);


	/**
	* @brief ゴールオブジェクト
	* @param[in] pos 位置
	* @param[out] p_obj ゴールオブジェクトのダブルポインタ
	*/
	void CreateGoalObject(
		const Vec3&pos,
		GoalObject**p_obj
	);


	/**
	* @brief 背景生成
	* @param[in] pos 位置
	*/
	void CreateBackGround(
		const Vec3 &pos
	);


	/**
	* @brief 板オブジェクト生成
	* @param[in] obj_param オブジェクトパラメータ構造体
	* @param[out] MapObjectBase**p_map_obj
	*/
	void CreateTaile(
		const ObjParameter&obj_param,
		MapObjectBase**p_map_obj,
		const Taile::Direction &dir = Taile::Direction::FLOOR
	);

	/**
	* @brief 板生成
	* @param[in] data オブジェクトパラメータ構造体
	* param[in]
	*/
	void CreateTaile2(
		const ObjParameter&data,
		std::weak_ptr<MapObjectBase>p_map_obj
	);


};

#endif