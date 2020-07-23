#pragma once
#include"../BulletBase/BulletBase.h"
#include"../../../Lib/3D/OBJ/OBJFile.h"
#include"../BulletData/BulletData.h"



/**
* @brief 敵の弾
*/
class EnemyBullet : public BulletBase{
public:


	/**
	* @brief コンストラクタ
	* @param[in] pos 位置
	* @param[in] speed 速度
	* @param[in] limit_distance 弾の制限距離
	* @param[in] dir 方向
	*/
	EnemyBullet(
		const ObjParameter&param,
		const BulletData&data
	);


	/**
	* @brief 更新 仮想関数
	*/
	void Update()override;


	/**
	* @brief 描画 仮想関数
	*/
	void Draw()override;


private:


	/**
	* @brief 衝突後の処理
	* @param[in] type 相手の衝突種類
	*/
	void HitAction(const CollisionObjectType&type)override;


	/**
	* @brief 球を返す
	*/
	Sphere GetSphere()override;


	/**
	* @brief 方向に弾を飛ばす
	*/
	void AddDirToPos();

private:

	//! 敵の位置
	Vec3 m_enemy_pos;

	//! オブジェクトパラメータ
	ObjParameter m_obj_param;
};