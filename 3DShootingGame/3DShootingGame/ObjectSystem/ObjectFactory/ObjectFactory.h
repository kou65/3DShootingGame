#pragma once
#include"../ObjectManager/ObjectManager.h"
#include"../../Lib/Vec3/Vec3.h"
#include"../../Lib/SingletonTemplate/SingletonTemplate.h"
#include"../../Lib/3D/Camera3D/Camera3D.h"



class ObjectFactory {
public:

	// 自機生成
	void CreatePlayer(
		const Vec3&pos,
		Camera3D*m_p_camera,
		ObjectFactory*factory
	);

	// 弾生成
	void CreateBullet(
		const Vec3 &pos,
		const float &speed,
		const float &distance
	);

	// 敵生成
	void CreateEnemy(
		const Vec3 &pos,
		const float &speed
	);

	// フィールド生成
	void CreateFiled();

};