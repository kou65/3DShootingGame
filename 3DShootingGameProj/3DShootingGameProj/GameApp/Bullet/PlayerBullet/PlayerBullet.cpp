#include"PlayerBullet.h"
#include"../../../Manager/CollisionManager/CollisionManager.h"



PlayerBullet::PlayerBullet(
	const ObjParameter&param,
	const BulletData&data
) {

	// オブジェクトパラメータ
	m_obj_param = param;

	// 位置
	m_pos = data.trans_data.pos;
	m_rotate = data.trans_data.rota;

	// 距離
	m_len_vec = m_pos;

	// 移動速度
	m_speed = data.speed;

	// 弾の制限距離
	m_limit_distance = data.distance_limit;

	// 形状
	m_shape_type = ShapeType::SPHERE;
}


void PlayerBullet::Update() {

	Rotation();
	AddMove();
	Limit(m_len_vec);

	// 位置移動
	m_obj_param.pos = m_pos;
}


void PlayerBullet::Draw() {

	Obj::GetInstance()->Draw(DrawStatus::LIGHT, m_obj_param, 8);
	//Obj::GetInstance()->DrawObjByNormalShader(m_obj_param);
}


void PlayerBullet::HitAction(const CollisionObjectType&type) {
	
	// 消滅
	m_is_active = false;
}


Sphere PlayerBullet::GetSphere(){

	Sphere s;

	s.vec = m_pos;
	s.vec /= 2;
	//s.vec.x += 0.5f;
	s.radian = 10.f;

	return s;
}



Cube PlayerBullet::GetCube() {

	Cube c;

	c.m_rect_3d.x = m_pos.x;
	c.m_rect_3d.y = m_pos.y;
	c.m_rect_3d.z = m_pos.z;

	c.m_rect_3d.width = 1;
	c.m_rect_3d.height = 1;
	c.m_rect_3d.depth = 1;

	return c;
}



void PlayerBullet::AddMove() {

	m_pos += m_dir * m_speed;

	m_dir.Init();
}


void PlayerBullet::Rotation() {
	
	// 行列
	D3DXMATRIX rot_m_x, rot_m_y, rot_m_z,total;
	D3DXMATRIX trans_m;

	// 初期化
	D3DXMatrixIdentity(&total);

	// 移動値
	Vec3 dir;

	dir.x = 0.f;
	dir.y = 0.f;
	dir.z = 1.f;

	// 正規化
	D3DXVec3Normalize(&dir, &dir);

	// 回転
	D3DXMatrixRotationX(&rot_m_x,D3DXToRadian(m_data.trans_data.rota.x));
	D3DXMatrixRotationY(&rot_m_y,D3DXToRadian(m_data.trans_data.rota.y));

	// 頂点変換
	D3DXVec3TransformNormal(&dir,&dir,&rot_m_y);
	D3DXVec3TransformNormal(&dir,&dir,&rot_m_x);

	// 移動値
	m_dir = dir;
}
