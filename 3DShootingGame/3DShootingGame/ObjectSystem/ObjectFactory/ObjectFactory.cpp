#include"ObjectFactory.h"
#include"../../GameApp/Bullet/Bullet.h"
#include"../../GameApp/Enemy/Enemy.h"
#include"../../GameApp/Player/Player.h"
#include"../../GameApp/Filed/Filed.h"



void ObjectFactory::CreatePlayer(
	const Vec3&pos,
	Camera3D*m_p_camera,
	ObjectFactory*factory
){

	ObjectManager::GetInstance()->InsertListEntry(
		new Player(
			Vec3(0.f, 0.f, 20.f),
			m_p_camera,
			this)
	);
}


void ObjectFactory::CreateBullet(
	const Vec3 &pos,
	const float &speed,
	const float &distance
) {

	ObjectManager::GetInstance()->InsertListEntry
	(new Bullet(
		Vec3(pos.x, pos.y, pos.z),
		0.f,
		speed,
		Vec3(distance + pos.x, distance + pos.y, distance + pos.z)
	));
}


void ObjectFactory::CreateEnemy(
	const Vec3 &pos,
	const float &speed
) {

	// 生成
	ObjectManager::GetInstance()->InsertListEntry(
		new Enemy(
			pos,
			speed
		));
}


void ObjectFactory::CreateFiled() {

	ObjectManager::GetInstance()->InsertListEntry(
		new Filed
	);
}