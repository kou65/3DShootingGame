#pragma once
#include"../EnemyBase/EnemyBase.h"
#include"../../BulletFactory/BulletFactory.h"
#include"../../Player/Player.h"



class HomingBulletEnemy : public EnemyBase{
public:

	const float INTERVAL = 600.f;

public:

	HomingBulletEnemy(
		BulletFactory*factory,
		Player*player
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
	BulletFactory*m_p_bullet_factory;

	// !プレイヤー
	Player*m_p_player;

	// !打ち出す時間
	float m_shot_timer;
};