#pragma once
#include"../../Lib/Vec3/Vec3.h"


struct Rect3D {
	float x;
	float y;
	float z;
	float width;
	float height;
	float front;
};


namespace Collision {

	/* 引数の説明
1,オブジェクト1の座標
2,オブジェクト2の座標
3,オブジェクト1の半径
4,オブジェクト2の半径
rad_1,rad2は半径の大きさ
(中心から〇を出したい場合は中心の座標を当たり判定に入れこむこと)
*/

	bool IsHitSphereAndSphere(
		Vec3 v1,
		Vec3 v2,
		float rad_1,
		float rad_2
	);



	// レイと球の衝突判定

	// lx, ly, lz : レイの始点
	// vx, vy, vz : レイの方向ベクトル
	// px, py, pz : 球の中心点の座標
	// r : 球の半径
	// q1x, q1y, q1z: 衝突開始点（戻り値）
	// q2x, q2y, q2z: 衝突終了点（戻り値）

	bool IsHitRayAndSphere(
		float lx, float ly, float lz,
		float vx, float vy, float vz,
		float px, float py, float pz,
		float r,
		float &q1x, float &q1y, float &q1z,
		float &q2x, float &q2y, float &q2z
	);


	/* 正方形と正方形の当たり判定 */
	bool IsHitCubeAndCube(
		const Rect3D&pos1,
		const Rect3D&pos2
	);


	bool IsHitRayAndPoint(
		Vec3&v1,
		Vec3&v2
	);

	bool IsHitCapsuleAndCapsule(

	);
}