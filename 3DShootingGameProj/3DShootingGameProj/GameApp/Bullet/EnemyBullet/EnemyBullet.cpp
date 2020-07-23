#include"EnemyBullet.h"
#include"../../../Manager/CollisionManager/CollisionManager.h"



EnemyBullet::EnemyBullet(
	const ObjParameter&param,
	const BulletData&data
) {

	// オブジェクトパラメータ
	m_obj_param = param;

	m_pos = data.trans_data.pos;
	m_enemy_pos = m_pos;
	m_speed = data.speed;
	m_dir = data.rot_dir;
	m_limit_distance = data.distance_limit;

	m_shape_type = ShapeType::SPHERE;
}


void EnemyBullet::Update() {

	AddDirToPos();
	Limit(m_enemy_pos);

	// 位置移動
	m_obj_param.pos = m_pos;
}


void EnemyBullet::Draw() {
	Obj::GetInstance()->Draw(DrawStatus::LIGHT, m_obj_param, 8);
	//Obj::GetInstance()->DrawObjByNormalShader(m_obj_param);
}


void EnemyBullet::HitAction(const CollisionObjectType&type) {

	// 消滅
	m_is_active = false;
}


Sphere EnemyBullet::GetSphere() {

	Sphere s;

	s.vec = m_pos;
	s.vec /= 2;
	s.radian = 10.f;

	return s;
}



void EnemyBullet::AddDirToPos() {

	// 方向がないなら
	if (m_dir.z <= 0.f) {
		m_dir.z = 1.f;
	}

	// 方向 * 速度
	m_pos += m_dir * m_speed;
}