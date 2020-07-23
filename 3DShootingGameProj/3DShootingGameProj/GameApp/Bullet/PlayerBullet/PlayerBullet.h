#ifndef BULLET_H
#define BULLET_H

#include"../../../Lib/3D/OBJ/OBJFile.h"
#include"../BulletBase/BulletBase.h"
#include"../BulletData/BulletData.h"



/**
* @brief 通常の弾クラス
*/
class PlayerBullet : public BulletBase{
public:

	/**
	* @brief コンストラクタ
	* @param[in] data 弾情報構造体
	*/
	PlayerBullet(
		const ObjParameter&param,
		const BulletData&data
	);


	/**
	* @brief 更新 仮想関数
	*/
	void Update()override;


	/**
	* @brief 描画　仮想関数
	*/
	void Draw()override;


	/**
	* @brief 衝突後の処理
	* @param[in] type 衝突種類
	*/
	void HitAction(const CollisionObjectType&type)override;


	/**
	* @brief 球情報を返す
	* @return Sphere 球構造体
	*/
	Sphere GetSphere()override;


	/**
	* @brief キューブ情報を返す
	* @return Cube キューブ構造体
	*/
	Cube GetCube()override;


	/**
	* @brief 移動加算
	*/
	void AddSpeed(const Vec3 &add_speed) {
		m_data.speed += add_speed;
	}

private:

	//! 最終速度
	const float FINAL_V = 10.f;

	//! 加速度
	const float ADD_V = 1.f;

private:

	/**
	* @brief 移動加算
	*/
	void AddMove();


	/**
	* @brief 回転
	*/
	void Rotation();

private:

	//! 弾のデータ
	BulletData m_data;

	//! 回転値
	D3DXMATRIX m_rot;

	//! 距離を測る相手
	Vec3 m_len_vec;

	//! オブジェクトパラメータ
	ObjParameter m_obj_param;
};

#endif