#include"../Enemy/Enemy.h"
#include"../../CollisionSystem/Shape/ShapeData/ShapeData.h"




Enemy::Enemy(
	D3DXVECTOR3 pos,
	float speed
){

	m_pos = pos;
	m_speed = speed;

	m_shape_type = ShapeType::SPHERE;

	// 衝突に追加
	CollisionManager::GetInstance()->Entry(
		CollisionObjectType::ENEMY,this);
}


void Enemy::Update() {

	// Move()
}


void Enemy::Draw(){

	ObjParameter param;

	param.register_obj_file_name = Const::Obj::CUBE;
	param.pos = m_pos;

	Obj::GetInstance()->DrawObjByNormalShader(param);

}


void Enemy::HitAction(const CollisionObjectType&type) {

	// 消滅
	m_is_active = false;
}


Sphere Enemy::GetSphere() {

	Sphere s;

	s.vec = m_pos;

	// 中心
	s.vec /= 2;

	s.radian = 30.f;

	return s;
}