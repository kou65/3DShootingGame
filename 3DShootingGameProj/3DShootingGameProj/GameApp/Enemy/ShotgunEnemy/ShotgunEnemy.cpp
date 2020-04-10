#include"ShotgunEnemy.h"
#include"../../../Lib/3D/OBJ/OBJFile.h"
#include"../../../CollisionSystem/CollisionManager/CollisionManager.h"



ShotgunEnemy::ShotgunEnemy(
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


void ShotgunEnemy::Update() {

}


void ShotgunEnemy::Draw() {


	ObjParameter param;

	param.pos = m_pos;
	param.register_obj_file_name = Const::Obj::CUBE;

	// 敵描画
	Obj::GetInstance()->ShaderDraw(param);
}



// 衝突後処理
void ShotgunEnemy::HitAction(const CollisionObjectType&type) {

	Exit();
}


Sphere ShotgunEnemy::GetSphere() {

	Sphere s;
	s.radian = 10.f;
	s.vec = m_pos;
	s.vec /= 2;

	return s;
}



void ShotgunEnemy::Shot() {


	for (int i = 0; i < 3; i++) {
		// 弾を生成
		m_p_obj_factory->CreateBullet(
			m_pos,
			BULLET_SPEED,
			200.f
		);
	}

}