#pragma once
#include"../EnemyBase/EnemyBase.h"
#include"../../BulletFactory/BulletFactory.h"
#include"../../Player/Player.h"



class HomingBulletEnemy : public EnemyBase{
public:

	HomingBulletEnemy(
		BulletFactory*factory,
		Player*player
	);

	void Update()override;

	void Draw()override;

	void Shot();

private:

	// 方向計算
	Vec3 CalcDirection();

private:

	BulletFactory*m_p_bullet_factory;
	Player*m_p_player;
};