#pragma once
#include"../../../Lib/3D/OBJ/OBJFile.h"
#include"../BulletBase/BulletBase.h"
#include"../BulletData/BulletData.h"



/**
* @brief ブロック破壊用の弾オブジェクト
*/
class BreakBullet : public BulletBase{
public:


	/**
	* @brief コンストラクタ
	* @param[in] bullet_data 弾データ
	*/
	BreakBullet(
		const ObjParameter &param,
		const BulletData&bullet_data
	);


	/**
	* @brief 更新 仮想関数
	*/
	void Update()override;


	/**
	* @brief 描画 仮想関数
	*/
	void Draw()override;


	/**
	* @brief 衝突後処理
	*/
	void HitAction(const CollisionObjectType &type)override;


	/**
	* @brief 球情報を返す
	*/
	Sphere GetSphere()override;

private:


	/**
	* @brief 最終的な加算 
	*/
	void AddDirToPos();


	/**
	* @brief 回転方向加算
	*/
	void AddRotDir();

private:

	//! 弾データ
	BulletData m_bullet_data;

	//! オブジェパラメータ
	ObjParameter m_obj_param;
};