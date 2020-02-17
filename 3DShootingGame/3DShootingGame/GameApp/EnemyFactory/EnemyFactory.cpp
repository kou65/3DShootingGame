#include"EnemyFactory.h"
#include"../Enemy/Enemy.h"



void EnemyFactory::Create(
	const Vec3 &pos,
	const float &speed
) {

	// 生成
	m_p_object_manager->InsertListEntry(
		new Enemy(
			pos,
			speed
		));

}