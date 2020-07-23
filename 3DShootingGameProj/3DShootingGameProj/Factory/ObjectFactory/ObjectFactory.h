

#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H

#include"../../Manager/ObjectManager/ObjectManager.h"
#include"../../Lib/3D/Camera3D/Camera3D.h"
#include"../../GameApp/Filed/Taile/Taile.h"
#include"../../Lib/Vec3/Vec3.h"
#include"../../GameApp/CharacterBase/CharacterBase.h"
#include"../../GameApp/Filed/BackGround/BackGround.h"
#include"../../GameApp/Enemy/EnemyBase/EnemyBase.h"
#include"../../Factory/EffectFactory/EffectFactory.h"
#include"../../GameApp/Bullet/BulletData/BulletData.h"
#include"../../GameApp/Bullet/PlayerBullet/PlayerBullet.h"
#include"../../GameApp/Bullet/EnemyBullet/EnemyBullet.h"
#include"../../GameApp/Bullet/BreakBullet/BreakBullet.h"
#include"../../GameApp/Block/Block.h"
#include"../../GameApp/GoalObject/GoalObject.h"



/**
* @brief 前方宣言
*/
class Player;
struct ObjectData;


/**
* @brief オブジェクトを生成し管理者に追加する工場クラス
*/
class ObjectFactory {
public:


	// 自機共有用
	void CreateSharedPlayer(
		const Vec3&pos,
		std::weak_ptr <Camera3D>&mp_camera,
		std::weak_ptr<Player>&p_player
	);


	/**
	* @brief hpui生成
	* @param[in] object_data オブジェクトデータポインタ
	*/
	void CreateHPUI(
		std::shared_ptr<ObjectData>object_data
	);


	/**
	* @brief 自機の弾生成
	* @param[in] data 弾データ
	*/
	void CreateBullet(
		const ObjParameter&param,
		const BulletData&data,
		std::weak_ptr<PlayerBullet>&p_out_bullet
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
		const BulletData&data,
		std::weak_ptr<EnemyBullet>&p_out_bullet
	);


	/**
	* @brief 破壊弾生成
	* @param[in] data 弾データ
	*/
	void CreateBreakBullet(
		const ObjParameter &param,
		const BulletData&data,
		std::weak_ptr<BreakBullet>&p_out_bullet
	);


	/**
	* @brief その方向へ弾を撃つ敵生成
	* @param[in] pos 位置
	* @param[in] p_chara キャラクターポインタ
	* @param[out] p_enemy_base 基底敵ダブルポインタ
	*/
	void CreateHEnemy(
		const Vec3&pos,
		std::weak_ptr<CharacterBase>p_chara,
		std::weak_ptr<EnemyBase>&p_enemy_base
	);


	/**
	* @brief ショットガンを撃つ敵
	* @param[in] pos 位置
	* @param[in] p_chara キャラクター基底ポインタ
	* @param[out] p_enemy_base 基底敵ダブルポインタ
	*/
	void CreateShotgunEnemy(
		const Vec3&pos,
		std::weak_ptr<CharacterBase>p_chara,
		std::weak_ptr<EnemyBase>&p_enemy_base
	);


	/**
	* @brief ブロック作成
	* @param[in] pos 位置
	* @param[out] p_out_cube ブロックポインタ
	*/
	void CreateBlock(
		const Vec3&pos,
		std::weak_ptr<Block>&p_out_cube
	);


	/**
	* @brief ゴールオブジェクト
	* @param[in] pos 位置
	* @param[out] p_obj ゴールオブジェクトのダブルポインタ
	*/
	void CreateGoalObject(
		const Vec3&pos,
		std::weak_ptr<GoalObject>&p_obj
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
		const ObjParameter&data,
		const Taile::Direction &dir,
		std::weak_ptr<MapObjectBase>&p_map_obj
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

private:
	EffectFactory m_effect;
};

#endif