#ifndef ENEMY_H
#define ENEMY_H

#include"../../ObjectSystem/Object3D/Object3D.h"
#include"../../Lib/3D/OBJ/OBJFile.h"
#include"../GameConstant/GameConstant.h"



class Enemy : public Object3D{
public:

	const int WIDTH_MOVE_RIGHT = 200;
	const int WIDTH_MOVE_LEFT = 100;
	

public:

	Enemy(
		D3DXVECTOR3 pos,
		float speed);


	void Update()override;

	void Draw()override;

private:

	void Move() {

		// 左まで来たら
		if (m_pos.x <= WIDTH_MOVE_LEFT) {
			
			// 反転
			m_speed *= -1.f;
		}
		else if (m_pos.x <= WIDTH_MOVE_RIGHT) {
			// 反転
			m_speed *= -1.f;
		}

		// 移動加算
		m_pos.x += m_speed;
	}

private:

	float m_speed;
};

#endif