#include"HomingBulletEnemy.h"
#include"../../../Lib/3D/OBJ/OBJFile.h"


HomingBulletEnemy::HomingBulletEnemy(
	ObjectFactory*factory,
	Player*player
) {

	// オブジェクト代入
	m_p_obj_factory = factory;
	m_p_player = player;
	m_shot_timer = 0.f;
}


void HomingBulletEnemy::Update() {

	// shot_timer加算
	AddShotTimer();

	// trueなら撃つ
	if (IsShot() == true) {
		Shot();
	}
}


void HomingBulletEnemy::Draw() {

	ObjParameter param;

	param.pos = m_pos;
	param.register_obj_file_name = Const::Obj::ENEMY_BULLETER;

	//Obj::GetInstance()->Draw();
}


bool HomingBulletEnemy::IsShot() {

	if (m_shot_timer > INTERVAL) {
		m_shot_timer = 0.f;

		return true;
	}

	return false;
}


void HomingBulletEnemy::AddShotTimer() {
	m_shot_timer++;
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
	m_p_obj_factory->CreateBullet(dir, 1.f, 300.f);
}
