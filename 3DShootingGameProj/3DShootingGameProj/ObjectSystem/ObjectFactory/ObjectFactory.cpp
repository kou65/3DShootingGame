#include"ObjectFactory.h"
#include"../../GameApp/Bullet/PlayerBullet/Bullet.h"
#include"../../GameApp/Enemy/Enemy.h"
#include"../../Manager/MapStructureManager/MapStructureManager.h"
#include"../../GameApp/HPUI/HPUI.h"
#include"../../GameApp/Filed/BackGround/BackGround.h"
#include"../../GameApp/Enemy/HomingBulletEnemy/HomingBulletEnemy.h"
#include"../../GameApp/Bullet/EnemyBullet/EnemyBullet.h"
#include"../../GameApp/Enemy/ShotgunEnemy/ShotgunEnemy.h"
#include"../../GameApp/GoalObject/GoalObject.h"
#include"../../ObjectSystem/ObjectData/ObjectData.h"
#include"../../GameApp/Bullet/BreakBullet/BreakBullet.h"
#include<memory>



void ObjectFactory::CreatePlayer(
	const Vec3&pos,
	std::shared_ptr<Camera3D>m_p_camera,
	std::shared_ptr<ObjectData>p_out_player
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


void ObjectFactory::CreateSharedPlayer(
	const Vec3&pos,
	std::weak_ptr <Camera3D> m_p_camera,
	std::weak_ptr<ObjectData2>p_player_data
) {

	//// 共有用作成
	//std::shared_ptr<Player>p_player = std::make_shared<Player>
	//	(pos,m_p_camera,p_player_data);
	//
	//// エントリー
	//ObjectManager::GetInstance()->SharedEntry(p_player);
	//
	//// ロックして共有用に変換
	//std::shared_ptr<ObjectData2>data = p_player_data.lock();
	//
	//data->p_player = p_player;
}



void ObjectFactory::CreateHPUI(
	std::shared_ptr<ObjectData>p_player
) {

	if (p_player == nullptr) {
		return;
	}

	ObjectManager::GetInstance()->Entry(
		new HPUI(p_player->p_player)
	);
}


void ObjectFactory::CreateBullet(
	const ObjParameter&param,
	const BulletData&data
) {

	ObjectManager::GetInstance()->Entry
	(new Bullet(
		param,
		data
	));
}


void ObjectFactory::CreateEnemyBullet(
	const ObjParameter&param,
	const BulletData&data
) {


	ObjectManager::GetInstance()->Entry
	(new EnemyBullet(
		param,
		data
	));
}


void ObjectFactory::CreateBreakBullet(
	const ObjParameter &param,
	const BulletData&data
) {

	ObjectManager::GetInstance()->Entry(
		new BreakBullet(param,data)
	);
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


void ObjectFactory::CreateHEnemy(
	const Vec3&pos,
	CharacterBase*p_data,
	EnemyBase**p_enemy_base
) {

	if (p_data == nullptr) {
		return;
	}

	EnemyBase * p_enemy = new HomingBulletEnemy(pos,this, p_data);

	// 生成
	ObjectManager::GetInstance()->Entry(
		p_enemy
	);

	// 追加
	*p_enemy_base = p_enemy;
}


void ObjectFactory::CreateShotgunEnemy(
	const Vec3&pos,
	CharacterBase*p_data,
	EnemyBase**p_enemy_base
) {
	if (p_data == nullptr) {
		return;
	}

	EnemyBase * p_enemy = new ShotgunEnemy(pos, this, p_data);

	// 生成
	ObjectManager::GetInstance()->Entry(
		p_enemy
	);

	// 追加
	*p_enemy_base = p_enemy;
}


void ObjectFactory::CreateCube(
	const Vec3&pos,
	Block**p_out_cube
) {

	// キューブ生成
	Block*p_cube = new Block(pos);

	// キューブ生成
	ObjectManager::GetInstance()->Entry(
		p_cube
	);

	// 外部にキューブ情報の参照を持たせる
	*p_out_cube = p_cube;
}


void ObjectFactory::CreateGoalObject(
	const Vec3&pos,
	GoalObject**p_obj
) {

	GoalObject * p_goal = new GoalObject(pos);

	ObjectManager::GetInstance()->Entry(
		p_goal
	);

	*p_obj = p_goal;
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

