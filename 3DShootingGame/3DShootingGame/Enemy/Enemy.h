#pragma once
#include"../Object3D/Object3D.h"
#include"../Lib/3D/OBJ/OBJFile.h"



class Enemy : public Object3D{
public:

	const int WIDTH_MOVE_RIGHT = 200;
	const int WIDTH_MOVE_LEFT = 100;
	

public:

	Enemy(
		D3DXVECTOR3 pos,
		float speed) {
		m_pos = pos;
		m_speed = speed;
	}


	void Update()override {

		Move();
	}

	void Draw()override {

	ObjFile::GetInstance()->DrawSubSet("note_book",m_pos.x,m_pos.y,m_pos.z);

	}

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