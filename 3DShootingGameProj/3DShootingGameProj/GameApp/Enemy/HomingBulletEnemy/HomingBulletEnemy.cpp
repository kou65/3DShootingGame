#include"HomingBulletEnemy.h"
#include"../../../Lib/3D/OBJ/OBJFile.h"
#include"../../../CollisionSystem/CollisionManager/CollisionManager.h"



HomingBulletEnemy::HomingBulletEnemy(
	const Vec3&create_pos,
	ObjectFactory*factory,
	CharacterBase*player
) {

	// 生成位置
	m_pos = create_pos;

	// オブジェクト代入
	m_p_obj_factory = factory;

	m_p_player = player;

	m_shot_timer = 0.f;

	// 衝突に追加
	CollisionManager::GetInstance()->Entry(
		CollisionObjectType::ENEMY_BULLET, this
	);

	// タグをセット
	SetTag(Object3DTag::BULLET_ENEMY);

	m_shape_type = ShapeType::SPHERE;
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
	param.register_obj_file_name = Const::Obj::CUBE;

	// 敵描画
	Obj::GetInstance()->ShaderDraw(param);
}


// 衝突後処理
void HomingBulletEnemy::HitAction(const CollisionObjectType&type) {

	Exit();
}


Sphere HomingBulletEnemy::GetSphere() {

	Sphere s;
	s.radian = 10.f;
	s.vec = m_pos;
	s.vec /= 2;

	return s;
}


bool HomingBulletEnemy::IsShot() {

	if (m_shot_timer > INTERVAL){
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

	// 三平方の定理で距離を出す
	float l = sqrtf((dir.x * dir.x) + (dir.y * dir.y) + (dir.z * dir.z));

	// 正規化
	dir.x = dir.x / l;
	dir.y = dir.y / l;
	dir.z = dir.z / l;

	// 弾を生成
	m_p_obj_factory->CreateEnemyBullet(
		m_pos,
		Vec3(-BULLET_SPEED, -BULLET_SPEED, -BULLET_SPEED),
		Vec3(200.f, 200.f, 200.f),
		dir
	);
}
