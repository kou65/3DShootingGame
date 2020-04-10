#pragma once
#include"../../Vec3/Vec3.h"


// カメラ情報を保存する構造体


struct CameraData {

	CameraData() {

		// 各初期化
		pos.Init();
		axis.Init();
		move_num.Init();
		rota.Init();
		look_at_point.Init();
		view_distance.Init();
		view_distance.z = -50.f;

		// 上方向だけ持たせる
		up_direction.Init();
		up_direction.y = 1.f;

		is_debug = false;
	}

	// 位置
	Vec3 pos;

	// 軸の位置
	Vec3 axis;

	// 移動値
	Vec3 move_num;

	// カメラの回転値
	Vec3 rota;

	// 注視点
	Vec3 look_at_point;

	// 上方向
	Vec3 up_direction;

	// カメラからの距離
	Vec3 view_distance;

	// デバッグモードか
	bool is_debug;
};