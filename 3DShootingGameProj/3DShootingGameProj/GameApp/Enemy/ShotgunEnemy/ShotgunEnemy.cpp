﻿#include"ShotgunEnemy.h"
#include"../../../Lib/3D/OBJ/OBJFile.h"
#include"../../../Manager/CollisionManager/CollisionManager.h"
#include"../../../Factory/EffectFactory/EffectFactory.h"



ShotgunEnemy::ShotgunEnemy(
	const Vec3&create_pos,
	ObjectFactory *factory,
	std::weak_ptr<CharacterBase>player,
	EffectFactory*p_effect
) {

	// 生成位置
	m_pos = create_pos;

	// メイン工場
	mp_obj_factory = factory;

	// エフェクト工場
	mp_effect = p_effect;

	// プレイヤー
	mp_player = player;

	// ショットタイマー
	m_shot_timer = 0.f;

	// タグをセット
	SetTag(Object3DTag::BULLET_ENEMY);

	m_shape_type = ShapeType::SPHERE;
}


void ShotgunEnemy::Update() {

	if (IsShot() == true) {
		Shot();
	}
}


void ShotgunEnemy::Draw() {


	ObjParameter param;

	param.pos = m_pos;
	param.register_obj_file_name = Const::Obj::CUBE;

	// 敵描画
	//Obj::GetInstance()->DrawObjByNormalShader(param);
	Obj::GetInstance()->Draw(DrawStatus::LIGHT, param, 8);
}



// 衝突後処理
void ShotgunEnemy::HitAction(const CollisionObjectType&type) {

	Exit();
	mp_effect->CreatePolygonEffect(m_pos);
}


Sphere ShotgunEnemy::GetSphere() {

	Sphere s;
	s.radian = 10.f;
	s.vec = m_pos;
	s.vec.x += m_pos.x / 2;
	s.vec.x += m_pos.y / 2;
	s.vec.x += m_pos.z / 2;

	return s;
}


void ShotgunEnemy::Shot() {


	const float BULLET_DIR_Y[3] = {
		355.f,
		0.f,
		5.f
	};

	// 弾データ
	BulletData data;

	data.distance_limit = Vec3(200.f, 200.f, 200.f);
	data.speed = Vec3(-BULLET_SPEED, -BULLET_SPEED, -BULLET_SPEED);
	data.trans_data.pos = m_pos;

	// パラメータ
	ObjParameter param(DrawStatus::NORMAL);
	// キューブ
	param.register_obj_file_name = Const::Obj::SPEHER;
	// 拡縮
	param.scale = Vec3(0.5f, 0.5f, 0.5f);
	// 位置
	param.pos = m_pos;

	// 行列
	D3DXMATRIX rot_m_x, rot_m_y;
	// 移動値
	Vec3 dir;


	for (int i = 0; i < 3; i++) {

		// 正規化
		D3DXMatrixIdentity(&rot_m_x);
		D3DXMatrixIdentity(&rot_m_y);

		data.trans_data.rota.y = BULLET_DIR_Y[i];
		data.trans_data.rota.x = 0.f;

		dir.x = 0.f;
		dir.y = 0.f;
		dir.z = 1.f;

		// 正規化
		D3DXVec3Normalize(&dir, &dir);

		// 回転
		D3DXMatrixRotationX(&rot_m_x, D3DXToRadian(data.trans_data.rota.x));
		D3DXMatrixRotationY(&rot_m_y, D3DXToRadian(data.trans_data.rota.y));

		// 頂点変換
		D3DXVec3TransformNormal(&dir, &dir, &rot_m_y);
		D3DXVec3TransformNormal(&dir, &dir, &rot_m_x);

		// データに方向代入
		data.rot_dir = dir;

		std::weak_ptr<EnemyBullet>p_bullet;

		// 弾を生成
		mp_obj_factory->CreateEnemyBullet(
			param,
			data,
			p_bullet
		);

		// 衝突に追加
		CollisionManager::GetInstance()->Entry(
			CollisionObjectType::ENEMY_BULLET,p_bullet
		);
	}
}




bool ShotgunEnemy::IsShot() {

	if (m_shot_timer > INTERVAL) {

		m_shot_timer = 0.f;
		return true;
	}

	m_shot_timer++;

	return false;
}