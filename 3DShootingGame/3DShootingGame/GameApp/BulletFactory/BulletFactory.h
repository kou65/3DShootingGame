
#ifndef BULLET_FACTORY_H
#define BULLET_FACTORY_H

#include"../Bullet/Bullet.h"
#include<vector>
#include"../../ObjectSystem/ObjectManager/ObjectManager.h"



class BulletFactory {
public:

	BulletFactory(ObjectManager*object_manager) {
	
		m_p_object_manager = object_manager;
	};

	void Create(
		const Vec3 &pos,
		const float &speed,
		const float &distance
	);

private:

	ObjectManager * m_p_object_manager;
};

#endif