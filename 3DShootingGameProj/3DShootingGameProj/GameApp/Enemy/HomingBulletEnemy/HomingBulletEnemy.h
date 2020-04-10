#pragma once
#include"../EnemyBase/EnemyBase.h"
#include"../../../ObjectSystem/ObjectFactory/ObjectFactory.h"
#include"../../Player/Player.h"



class HomingBulletEnemy : public EnemyBase{
public:

	// 撃つ間隔
	const float INTERVAL = 40.f;
	const float BULLET_SPEED = 2.f;

public:

	HomingBulletEnemy(
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

private:

	// 間隔の時間を加算する
	bool IsShot();

	// 方向計算
	Vec3 CalcDirection();

	// 撃つ
	void Shot();

	// 時間計算
	void AddShotTimer();

private:

	// !弾生成工場
	ObjectFactory*m_p_obj_factory;

	// !プレイヤー
	CharacterBase*m_p_player;

	// !打ち出す時間
	float m_shot_timer;
};