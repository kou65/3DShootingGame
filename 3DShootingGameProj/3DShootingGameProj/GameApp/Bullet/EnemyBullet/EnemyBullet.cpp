#include"EnemyBullet.h"
#include"../../../CollisionSystem/CollisionManager/CollisionManager.h"



EnemyBullet::EnemyBullet(
	const Vec3 &pos,
	const Vec3 &speed,
	const Vec3 &limit_distance,
	const Vec3&dir
) {

	m_pos = pos;
	m_enemy_pos = m_pos;
	m_speed = speed;
	m_dir = dir;
	m_limit_distance = limit_distance;


	CollisionManager::GetInstance()->Entry(
		CollisionObjectType::ENEMY_BULLET, this
	);

	m_shape_type = ShapeType::SPHERE;
}


void EnemyBullet::Update() {

	AddDirToPos();
	Limit(m_enemy_pos);
}


void EnemyBullet::Draw() {

	ObjParameter param;
	param.register_obj_file_name = Const::Obj::SPEHER;
	param.pos = m_pos;

	Obj::GetInstance()->ShaderDraw(param);
}


void EnemyBullet::HitAction(const CollisionObjectType&type) {

	// 消滅
	m_is_active = false;
}


Sphere EnemyBullet::GetSphere() {

	Sphere s;

	s.vec = m_pos;
	s.vec /= 2;
	s.radian = 30.f;

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