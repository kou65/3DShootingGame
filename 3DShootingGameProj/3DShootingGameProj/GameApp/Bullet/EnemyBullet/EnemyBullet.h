#pragma once
#include"../BulletBase/BulletBase.h"
#include"../../../ObjectSystem/ObjectData/ObjectData.h"



class EnemyBullet : public BulletBase{
public:

	EnemyBullet(
		const Vec3 &pos,
		const Vec3 &speed,
		const Vec3 &limit_distance,
		const Vec3&dir
	);

	void Update()override;

	void Draw()override;

private:

	// �Փˌ�̏���
	void HitAction(const CollisionObjectType&type)override;

	// ��
	Sphere GetSphere()override;

	// �����ɒe���΂�
	void AddDirToPos();

private:
	Vec3 m_enemy_pos;

};