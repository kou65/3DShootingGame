#ifndef BULLET_H
#define BULLET_H

#include"../../CollisionSystem/CollisionObject3DBase/CollisionObject3DBase.h"
#include<math.h>
#include"../../Lib/Vec3/Vec3.h"



class Bullet : public CollisionObject3DBase{
public:

	Bullet(
	const Vec3&pos, 
		const float &radius,
		const float &speed,
		const Vec3&distance_limit_num = Vec3(100.f,100.f,100.f)
	);


	// 更新
	void Update()override;

	// 描画
	void Draw()override;

	// 衝突後の処理
	void HitAction()override;

	Sphere GetSphere()override;

	// 移動加算
	void AddSpeed(const float &add_speed) {
		m_speed += add_speed;
	}

	void Exit();
private:

	const float FINAL_V = 10.f;
	const float ADD_V = 1.f;

private:

	void AddMove();

	void Rotation();

private:

	// 角度
	float m_radius;

	// 弾の速度
	float m_speed;

	// 弾距離制限値
	Vec3 m_distance_limit_num;

	// 移動値
	Vec3 m_move;

	// 回転
	Vec3 m_vec_rot;

	// 回転値
	D3DXMATRIX m_rot;
};

#endif