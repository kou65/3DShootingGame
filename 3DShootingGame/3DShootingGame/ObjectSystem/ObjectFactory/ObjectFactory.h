﻿

#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H

#include"../ObjectManager/ObjectManager.h"
#include"../../Lib/3D/Camera3D/Camera3D.h"
#include"../../GameApp/Filed/Taile/Taile.h"
#include"../../Lib/Vec3/Vec3.h"
#include"../../GameApp/CharacterBase/CharacterBase.h"
#include"../../GameApp/Filed/BackGround/BackGround.h"


class Player;
struct ObjectData;


class ObjectFactory {
public:


	// 自機生成
	// オブジェクトデータ参照
	void CreatePlayer(
		const Vec3&pos,
		Camera3D*m_p_camera,
		ObjectData*p_player_data
	);


	// hpui生成
	void CreateHPUI(
		ObjectData*p_player_data
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


	// 背景生成
	void CreateBackGround(
		const Vec3 &pos
	);


	// 床生成
	void CreateTaile(
		const ObjParameter&data,
		MapObjectBase**p_map_obj,
		const Taile::Direction &dir = Taile::Direction::FLOOR
	);

	// 板生成
	void CreateTaile2(
		const ObjParameter&data,
		std::weak_ptr<MapObjectBase>p_map_obj
	);


	// フィールド生成
	void CreateFiled(ObjectData*p_chara);

};

#endif