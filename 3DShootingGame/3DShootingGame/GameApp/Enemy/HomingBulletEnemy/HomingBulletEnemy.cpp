#include"HomingBulletEnemy.h"



HomingBulletEnemy::HomingBulletEnemy(
	BulletFactory*factory,
	Player*player
) {

	// オブジェクト代入
	m_p_bullet_factory = factory;
	m_p_player = player;
}


void HomingBulletEnemy::Update() {

}


void HomingBulletEnemy::Draw() {

}


Vec3 HomingBulletEnemy::CalcDirection() {

	Vec3 dir;

	dir.x = m_pos.x - m_p_player->GetPos().x;
	dir.y = m_pos.y - m_p_player->GetPos().y;
	dir.z = m_pos.z - m_p_player->GetPos().z;

	return dir;
}


void HomingBulletEnemy::Shot(){

	Vec3 dir = CalcDirection();

	// 弾を生成
	m_p_bullet_factory->Create(dir, 1.f, 300.f);
}
