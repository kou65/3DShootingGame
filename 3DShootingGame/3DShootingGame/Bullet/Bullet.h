#pragma once
#include"../Object3D/Object3D.h"
#include"../Lib/3D/OBJ/OBJFile.h"
#include<math.h>



class Bullet : public Object3D{
public:

	static const int LIMIT_M = 100;

public:

	Bullet(
		const float &pos_x,
		const float &pos_y,
		const float &pos_z,
		const float &radius,
		const float &speed
	);


	// 更新
	void Update()override;

	// 描画
	void Draw()override;

	// 移動加算
	void AddSpeed(const float &add_speed) {
		m_speed += add_speed;
	}

	void Exit();

private:

	void Move();

private:

	// 位置
	D3DXVECTOR3 m_pos;

	// 角度
	float m_radius;

	// 弾の速度
	float m_speed;
};