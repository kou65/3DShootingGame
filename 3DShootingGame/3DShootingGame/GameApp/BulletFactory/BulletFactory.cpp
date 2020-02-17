#include"BulletFactory.h"




void BulletFactory::Create(
	const Vec3 &pos,
	const float &speed,
	const float &distance
) {

	m_p_object_manager->InsertListEntry
	(new Bullet(
		Vec3(pos.x, pos.y, pos.z),
		0.f,
		speed,
	Vec3(distance + pos.x,distance + pos.y,distance + pos.z)
	));

}
