#pragma once
#include"../../ObjectSystem/ObjectManager/ObjectManager.h"
#include"../../Lib/Vec3/Vec3.h"


class EnemyFactory {
public:

	// 生成
	void Create(
		const Vec3 &pos,
		const float &speed
	);

private:

	ObjectManager *m_p_object_manager;

};