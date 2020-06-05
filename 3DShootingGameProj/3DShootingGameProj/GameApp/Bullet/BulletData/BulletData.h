#pragma once
#include"../../../Lib/Vec3/Vec3.h"
#include"../../../Lib/TransformData/TransformData3D.h"



/**
* @brief 弾情報を格納する構造体
*/
struct BulletData {


	/**
	* brief コンストラクタ
	*/
	BulletData() {
		distance_limit = Vec3(100.f, 100.f, 100.f);
		speed = Vec3(0.f,0.f,0.f);
	}

	//! 移動回転拡縮データ
	TransformData3D trans_data;

	//! 距離制限
	Vec3 distance_limit;

	//! 速度
	Vec3 speed;

	//! 弾の回転方向
	Vec3 rot_dir;
};