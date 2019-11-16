#pragma once
#include"../Bullet/Bullet.h"
#include<vector>
#include"../ObjectManager/ObjectManager.h"



class BulletFactory {
public:

	BulletFactory(ObjectManager*object_manager) {
	
		m_p_object_manager = object_manager;
	};

	void Create(
		const float &pos_x,
		const float &pos_y,
		const float &pos_z,
		const float &speed
	);

private:

	ObjectManager * m_p_object_manager;
};