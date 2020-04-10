#pragma once
#include"../EnemyBase/EnemyBase.h"
#include"../../../ObjectSystem/ObjectFactory/ObjectFactory.h"



class ShotgunEnemy : public EnemyBase{
public:

	// 弾の速度
	const float BULLET_SPEED = 2.f;

public:

	ShotgunEnemy(
		const Vec3&create_pos,
		ObjectFactory*factory,
		CharacterBase*player
	);

	// 更新
	void Update()override;

	// 描画
	void Draw()override;

	// 衝突後処理
	void HitAction(const CollisionObjectType&type)override;

	// 球を返す
	Sphere GetSphere()override;

	void Shot();

private:


	// !弾生成工場
	ObjectFactory*m_p_obj_factory;

	// !プレイヤー
	CharacterBase*m_p_player;

	// !打ち出す時間
	float m_shot_timer;
};