#include"../Enemy/Enemy.h"



Enemy::Enemy(
	D3DXVECTOR3 pos,
	float speed) {

	m_pos = pos;
	m_speed = speed;
}


void Enemy::Update() {

	// Move()
}


void Enemy::Draw(){

	ObjParameter param;

	param.register_name = Const::Graph::ENEMY;
	param.pos = m_pos;

	ObjFile::GetInstance()->Draw(param);

}