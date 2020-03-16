#pragma once
#include"../EnemyBase/EnemyBase.h"
#include"../../../ObjectSystem/ObjectFactory/ObjectFactory.h"
#include"../../Player/Player.h"



class HomingBulletEnemy : public EnemyBase{
public:

	const float INTERVAL = 600.f;

public:

	HomingBulletEnemy(
		ObjectFactory*factory,
		CharacterBase*player
	);

	// 更新
	void Update()override;

	// 描画
	void Draw()override;

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