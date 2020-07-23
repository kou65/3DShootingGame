#include"BreakBullet.h"
#include"../../../Manager/CollisionManager/CollisionManager.h"



BreakBullet::BreakBullet(
	const ObjParameter &param,
	const BulletData&bullet_data
) {

	// パラメータ
	m_obj_param = param;

	// 位置
	m_length_pos = bullet_data.trans_data.pos;
	m_pos = bullet_data.trans_data.pos;

	m_limit_distance = bullet_data.distance_limit;

	m_bullet_data = bullet_data;

	// 加算
	AddRotDir();

	// 形状
	m_shape_type = ShapeType::SPHERE;
}


void BreakBullet::Update() {

	// 距離制限
	Limit(m_length_pos);

	// 位置加算
	AddDirToPos();

	// 位置移動
	m_obj_param.pos = m_pos;
}


void BreakBullet::Draw() {

	// 敵描画
	//Obj::GetInstance()->DrawObjByNormalShader(m_obj_param);
	Obj::GetInstance()->Draw(DrawStatus::LIGHT, m_obj_param, 8);
}


void BreakBullet::HitAction(const CollisionObjectType &type) {

	switch (type)
	{
	case CollisionObjectType::BLOCK:

		// 解放
		Exit();

		break;

	default:
		break;
	}
}


Sphere BreakBullet::GetSphere() {

	// 中心位置から
	Sphere s;

	s.vec = m_pos;
	s.radian = 10.f;

	return s;
}


void BreakBullet::AddRotDir() {

	// 行列
	D3DXMATRIX mat_rot_m_x, mat_rot_m_y;

	// 移動値
	Vec3 dir;

	// 正規化
	D3DXMatrixIdentity(&mat_rot_m_x);
	D3DXMatrixIdentity(&mat_rot_m_y);

	// 方向
	m_dir.x = 0.f;
	m_dir.y = 0.f;
	m_dir.z = 1.f;

	// 正規化
	D3DXVec3Normalize(&m_dir, &m_dir);

	// 回転
	D3DXMatrixRotationX(&mat_rot_m_x, D3DXToRadian(m_bullet_data.rot_dir.x));
	D3DXMatrixRotationY(&mat_rot_m_y, D3DXToRadian(m_bullet_data.rot_dir.y));

	// 頂点変換
	D3DXVec3TransformNormal(&m_dir, &m_dir, &mat_rot_m_y);
	D3DXVec3TransformNormal(&m_dir, &m_dir, &mat_rot_m_x);

}


void BreakBullet::AddDirToPos() {

	// 方向 * 速度
	m_pos += m_dir * m_bullet_data.speed;
}