#ifndef BULLET_H
#define BULLET_H

#include"BulletBase/BulletBase.h"




class Bullet : public BulletBase{
public:

	Bullet(
	const Vec3&pos,
		const float &speed,
		const Vec3&distance_limit_num = Vec3(100.f,100.f,100.f)
	);


	// 更新
	void Update()override;

	// 描画
	void Draw()override;

	// 衝突後の処理
	void HitAction(const CollisionObjectType&type)override;

	// 球
	Sphere GetSphere()override;

	// キューブ
	Cube GetCube()override;

	// 移動加算
	void AddSpeed(const float &add_speed) {
		m_speed += add_speed;
	}

private:

	const float FINAL_V = 10.f;
	const float ADD_V = 1.f;

private:

	void AddMove();

	void Rotation();

private:

	// 弾の速度
	float m_speed;

	// 回転
	Vec3 m_vec_rot;

	// 回転値
	D3DXMATRIX m_rot;

	// 距離を測る相手
	Vec3 m_len_vec;
};

#endif