#include"ObjectFactory.h"
#include"../../GameApp/Bullet/Bullet.h"
#include"../../GameApp/Enemy/Enemy.h"
#include"../../GameApp/Filed/Filed.h"
#include"../../GameApp/HPUI/HPUI.h"
#include"../../GameApp/Filed/BackGround/BackGround.h"
#include"../../ObjectSystem/ObjectData/ObjectData.h"



void ObjectFactory::CreatePlayer(
	const Vec3&pos,
	Camera3D*m_p_camera,
	ObjectData*p_out_player
){

	// 生成してそれぞれに渡す
	Player*p = new Player(
		pos,
		m_p_camera,
		this
	);

	// オブジェクトに登録
	ObjectManager::GetInstance()->Entry(
		p
	);

	// 参照を外へ出す
	p_out_player->p_player = p;
}


// hpui生成
void ObjectFactory::CreateHPUI(
	ObjectData*p_player
) {

	if (p_player == nullptr) {
		return;
	}

	ObjectManager::GetInstance()->Entry(
		new HPUI(p_player->p_player)
	);
}


void ObjectFactory::CreateBullet(
	const Vec3 &pos,
	const float &speed,
	const float &distance
) {

	ObjectManager::GetInstance()->Entry
	(new Bullet(
		pos,
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
	ObjectManager::GetInstance()->Entry(
		new Enemy(
			pos,
			speed
		));
}


void ObjectFactory::CreateBackGround(
	const Vec3 &pos
) {

	// 生成
	ObjectManager::GetInstance()->Entry(
		new BackGround(
			pos
		));
}


void ObjectFactory::CreateTaile(
	const ObjParameter&data,
	MapObjectBase**p_map_obj,
	const Taile::Direction &dir
) {

	Taile*p_taile = new Taile(data,dir);

	ObjectManager::GetInstance()->Entry(
		p_taile
	);

	// マップオブジェクト代入
	*p_map_obj = p_taile;
}


void ObjectFactory::CreateTaile2(
	const ObjParameter&data,
	std::weak_ptr<MapObjectBase>p_map_obj
) {

	// 共有用生成
	std::shared_ptr<Taile>p_taile = std::make_shared<Taile>(data);

	ObjectManager::GetInstance()->SharedEntry(
		p_taile
	);

	// マップオブジェクト代入
	p_map_obj = p_taile;
}


void ObjectFactory::CreateFiled(ObjectData*p_chara) {

	if (p_chara == nullptr) {
		return;
	}

	ObjectManager::GetInstance()->Entry(
		new Filed(this,p_chara->p_player)
	);
}