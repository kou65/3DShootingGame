#include"SniperEnemy.h"
#include"../../../Lib/3D/OBJ/OBJFile.h"
#include"../../../Manager/CollisionManager/CollisionManager.h"



SniperEnemy::SniperEnemy(
	const Vec3&create_pos,
	ObjectFactory *p_factory,
	std::weak_ptr<CharacterBase>p_chara,
	EffectFactory*p_effect
) {

	// 生成位置
	m_pos = create_pos;

	// オブジェクト代入
	mp_obj_factory = p_factory;

	// エフェクト工場
	mp_effect = p_effect;

	mp_player = p_chara;

	m_shot_timer = 0.f;

	// タグをセット
	SetTag(Object3DTag::BULLET_ENEMY);

	m_shape_type = ShapeType::SPHERE;
}


void SniperEnemy::Update() {

	// shot_timer加算
	AddShotTimer();

	// trueなら撃つ
	if (IsShot() == true) {
		Shot();
	}
}


void SniperEnemy::Draw() {

	ObjParameter param;

	param.pos = m_pos;
	param.register_obj_file_name = Const::Obj::CUBE;
	param.color = D3DXVECTOR4(0.f, 1.f, 0.f, 1.f);

	// 敵描画
	Obj::GetInstance()->Draw(DrawStatus::LIGHT,param,8);
	//Obj::GetInstance()->DrawObjByNormalShader(param);
}


// 衝突後処理
void SniperEnemy::HitAction(const CollisionObjectType&type) {

	mp_effect->CreatePolygonEffect(m_pos);
	Exit();
}


Sphere SniperEnemy::GetSphere() {

	Sphere s;
	s.radian = 10.f;
	s.vec = m_pos;
	s.vec /= 2;

	return s;
}


bool SniperEnemy::IsShot() {

	if (m_shot_timer > INTERVAL){
		m_shot_timer = 0.f;

		return true;
	}

	return false;
}


void SniperEnemy::AddShotTimer() {
	m_shot_timer++;
}


Vec3 SniperEnemy::CalcDirection() {

	Vec3 dir;

	// プレイヤーポインタを返す
	std::shared_ptr<CharacterBase>p_player =
		mp_player.lock();

	if (p_player == nullptr) {
		return dir;
	}

	// 方向算出
	dir.x = m_pos.x - p_player->GetPos().x;
	dir.y = m_pos.y - p_player->GetPos().y;
	dir.z = m_pos.z - p_player->GetPos().z;

	return dir;
}


void SniperEnemy::Shot(){

	Vec3 dir = CalcDirection();

	// 三平方の定理で距離を出す
	float l = sqrtf((dir.x * dir.x) + (dir.y * dir.y) + (dir.z * dir.z));

	// 正規化
	dir.x = dir.x / l;
	dir.y = dir.y / l;
	dir.z = dir.z / l;

	BulletData data;
	data.trans_data.pos = m_pos;
	// 反対方向
	data.speed = Vec3(-BULLET_SPEED, -BULLET_SPEED, -BULLET_SPEED);
	data.distance_limit = Vec3(200.f, 200.f, 200.f);
	data.rot_dir = dir;

	ObjParameter param;
	// 球
	param.register_obj_file_name = Const::Obj::SPEHER;
	// 拡縮
	param.scale = Vec3(0.5f, 0.5f, 0.5f);
	// 位置
	param.pos = m_pos;

	if (mp_obj_factory != nullptr) {

		std::weak_ptr<EnemyBullet>p_bullet;

		// 弾を生成
		mp_obj_factory->CreateEnemyBullet(
			param,
			data,
			p_bullet
		);

		// 衝突に登録
		CollisionManager::GetInstance()->Entry(
			CollisionObjectType::ENEMY_BULLET, p_bullet
		);
	}
}
