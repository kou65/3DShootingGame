#pragma once
#include"../../../CollisionSystem/CollisionObject3DBase/CollisionObject3DBase.h"
#include"../../../Lib/Vec3/Vec3.h"
#include<math.h>


// �e�̊��N���X


class BulletBase : public CollisionObject3DBase{
public:


	// �p�x�̑΂��Ĉړ����x���|���Đi�܂���
	//m_pos.z += cosf(m_radius) * m_speed;
	//m_pos.y += sinf(m_radius) * m_speed;


protected:

	void Limit();

	void Limit(Vec3&vec);

protected:

	// �e�̕���
	Vec3 m_dir;

	// �e�̑��x
	Vec3 m_speed;

	// �e�̗L���͈�
	Vec3 m_limit_distance;
};