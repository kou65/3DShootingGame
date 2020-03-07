﻿#include"Bullet.h"
#include"../../Lib/3D/OBJ/OBJFile.h"
#include"../../CollisionSystem/CollisionManager/CollisionManager.h"



Bullet::Bullet(
	const Vec3&pos,
	const float &radius,
	const float &speed,
	const Vec3&distance_limit_num
) {

	// 位置
	m_pos.x = pos.x;
	m_pos.y = pos.y;
	m_pos.z = pos.z;

	// 角度
	m_radius = radius;

	// 移動速度
	m_speed = speed;

	// 弾の制限距離
	m_distance_limit_num = distance_limit_num;

	m_shape_type = ShapeType::SPHERE;

	// 弾を衝突物として登録
	CollisionManager::GetInstance()
		->Entry(CollisionObjectType::BULLET,this);
}


void Bullet::Update() {

	Rotation();
	AddMove();
	Exit();
}


void Bullet::Draw() {

	ObjParameter param;
	param.register_obj_file_name = Const::Obj::BULLET;
	param.pos = m_pos;

	Obj::GetInstance()->ShaderDraw(param);
}


void Bullet::HitAction() {
	
	// 消滅
	m_is_active = false;
}


Sphere Bullet::GetSphere(){

	Sphere s;

	s.vec = m_pos;
	s.radian = 30.f;

	return s;
}


void Bullet::AddMove() {

	m_radius = 0.f;

	m_pos += m_move * m_speed;

	m_move.Init();
}


void Bullet::Rotation() {
	
	// 行列
	D3DXMATRIX rot_m_x, rot_m_y, rot_m_z,total;
	D3DXMATRIX trans_m;

	// 初期化
	D3DXMatrixIdentity(&total);

	// 移動値
	Vec3 move;

	move.x = 0.f;
	move.y = 0.f;
	move.z = 1.f;

	// 正規化
	D3DXVec3Normalize(&move, &move);

	// 回転
	D3DXMatrixRotationX(&rot_m_x,D3DXToRadian(m_vec_rot.x));
	D3DXMatrixRotationY(&rot_m_y,D3DXToRadian(m_vec_rot.y));

	// 頂点変換
	D3DXVec3TransformNormal(&move,&move,&rot_m_y);
	D3DXVec3TransformNormal(&move,&move,&rot_m_x);

	// 移動値
	m_move = move;

	// 角度の対して移動速度を掛けて進ませる
	//m_pos.z += cosf(m_radius) * m_speed;
	//m_pos.y += sinf(m_radius) * m_speed;

}


void Bullet::Exit() {

	if (m_pos.x >= m_distance_limit_num.x ||
		m_pos.y >= m_distance_limit_num.y ||
		m_pos.z >= m_distance_limit_num.z) {
		m_is_active = false;
	}

}