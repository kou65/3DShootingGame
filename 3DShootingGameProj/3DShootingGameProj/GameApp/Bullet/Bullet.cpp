#include"Bullet.h"
#include"../../Lib/3D/OBJ/OBJFile.h"
#include"../../CollisionSystem/CollisionManager/CollisionManager.h"



Bullet::Bullet(
	const Vec3&pos,
	const float&speed,
	const Vec3&distance_limit
) {

	// 位置
	m_pos.x = pos.x;
	m_pos.y = pos.y;
	m_pos.z = pos.z;

	// 距離
	m_len_vec = pos;

	// 移動速度
	m_speed = speed;

	// 弾の制限距離
	m_limit_distance = distance_limit;

	// 形状
	m_shape_type = ShapeType::SPHERE;

	// 弾を衝突物として登録
	CollisionManager::GetInstance()
		->Entry(CollisionObjectType::PLAYER_BULLET,this);
}


void Bullet::Update() {

	Rotation();
	AddMove();
	Limit(m_len_vec);
}


void Bullet::Draw() {

	ObjParameter param;
	param.register_obj_file_name = Const::Obj::SPEHER;
	param.pos = m_pos;

	Obj::GetInstance()->ShaderDraw(param);
}


void Bullet::HitAction(const CollisionObjectType&type) {
	
	// 消滅
	m_is_active = false;
}


Sphere Bullet::GetSphere(){

	Sphere s;

	s.vec = m_pos;
	s.vec /= 2;
	s.radian = 30.f;

	return s;
}



Cube Bullet::GetCube() {

	Cube c;

	c.m_rect_3d.x = m_pos.x;
	c.m_rect_3d.y = m_pos.y;
	c.m_rect_3d.z = m_pos.z;

	c.m_rect_3d.width = 1;
	c.m_rect_3d.height = 1;
	c.m_rect_3d.depth = 1;

	return c;
}



void Bullet::AddMove() {

	m_pos += m_dir * m_speed;

	m_dir.Init();
}


void Bullet::Rotation() {
	
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
	D3DXMatrixRotationX(&rot_m_x,D3DXToRadian(m_vec_rot.x));
	D3DXMatrixRotationY(&rot_m_y,D3DXToRadian(m_vec_rot.y));

	// 頂点変換
	D3DXVec3TransformNormal(&dir,&dir,&rot_m_y);
	D3DXVec3TransformNormal(&dir,&dir,&rot_m_x);

	// 移動値
	m_dir = dir;

}
