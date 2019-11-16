#include"BulletFactory.h"




void BulletFactory::Create(
	const float &pos_x,
	const float &pos_y,
	const float &pos_z,
	const float &speed
) {

	m_p_object_manager->Entry(new Bullet(pos_x, pos_y, pos_z,0.f,speed));

}
