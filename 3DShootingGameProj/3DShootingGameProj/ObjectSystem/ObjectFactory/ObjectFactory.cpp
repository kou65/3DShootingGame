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



void ObjectFactory::CreateSharedPlayer(
	const Vec3&pos,
	std::weak_ptr <Camera3D>&mp_camera,
	std::weak_ptr<Player>&p_player
) {

	// カメラ
	std::shared_ptr<Camera3D>cam = 
	mp_camera.lock();

	// 共有用作成
	std::shared_ptr<Player>p_out_player = std::make_shared<Player>
		(pos,cam,this);

	// 外部出力
	p_player = p_out_player;

	// 所有権を渡し、追加
	ObjectManager::GetInstance()->EntrySharedObj(
		std::move(p_out_player)
	);
}


void ObjectFactory::CreateHPUI(
	std::shared_ptr<ObjectData>p_data
) {

	if (p_data == nullptr) {
		return;
	}

	ObjectManager::GetInstance()->EntryUniqueObj(
		std::make_unique<HPUI>(p_data->p_player)
	);
}


void ObjectFactory::CreateBullet(
	const ObjParameter&param,
	const BulletData&data
) {

	ObjectManager::GetInstance()->EntryUniqueObj
	(
		std::make_unique<Bullet>(param, data)
	);
}


void ObjectFactory::CreateEnemyBullet(
	const ObjParameter&param,
	const BulletData&data
) {


	ObjectManager::GetInstance()->EntryUniqueObj
	(
		std::make_unique<EnemyBullet>(
			param,
			data
		)
	);
}


void ObjectFactory::CreateBreakBullet(
	const ObjParameter &param,
	const BulletData&data
) {

	ObjectManager::GetInstance()->EntryUniqueObj(
		std::make_unique<BreakBullet>(param, data)
	);
}


void ObjectFactory::CreateBackGround(
	const Vec3 &pos
) {

	// 生成
	ObjectManager::GetInstance()->EntryUniqueObj(
		std::make_unique<BackGround>(pos)
	);
}


void ObjectFactory::CreateHEnemy(
	const Vec3&pos,
	std::weak_ptr<CharacterBase>p_data,
	std::weak_ptr<EnemyBase>&p_enemy_base
) {

	if (p_data.lock() == nullptr) {
		return;
	}

	std::shared_ptr<EnemyBase>p_enemy =
		std::make_shared<HomingBulletEnemy>(pos, this, p_data);

	// 生成
	ObjectManager::GetInstance()->EntrySharedObj(
		std::move(p_enemy)
	);

	// 追加
	p_enemy_base = p_enemy;
}


void ObjectFactory::CreateShotgunEnemy(
	const Vec3&pos,
	std::weak_ptr<CharacterBase>p_data,
	std::weak_ptr<EnemyBase>&p_enemy_base
) {
	if (p_data.lock() == nullptr){
		return;
	}

	std::shared_ptr<EnemyBase>p_enemy = 
		std::make_shared<ShotgunEnemy>(pos, this, p_data);

	// 参照値に追加
	p_enemy_base = p_enemy;

	// 生成
	ObjectManager::GetInstance()->EntrySharedObj(
		std::move(p_enemy)
	);

}


void ObjectFactory::CreateBlock(
	const Vec3&pos,
	std::weak_ptr<Block>&p_out_cube
) {

	// キューブ生成
	std::shared_ptr<Block>p_block =
		std::make_shared<Block>(pos);

	// 外部にキューブ情報の参照を持たせる
	p_out_cube = p_block;

	// キューブ生成
	ObjectManager::GetInstance()->EntrySharedObj(
		std::move(p_block)
	);
}


void ObjectFactory::CreateGoalObject(
	const Vec3&pos,
	std::weak_ptr<GoalObject>p_obj
) {

	std::shared_ptr<GoalObject>p_goal_obj =
		std::make_shared<GoalObject>(pos);

	// 参照値に代入
	p_obj = p_goal_obj;

	ObjectManager::GetInstance()->EntrySharedObj(
		std::move(p_goal_obj)
	);

}



void ObjectFactory::CreateTaile(
	const ObjParameter&data,
	const Taile::Direction &dir,
	std::weak_ptr<MapObjectBase>&p_map_obj
) {

	std::shared_ptr<Taile>p_taile =
	std::make_shared<Taile>(data, dir);

	// 参照値追加
	p_map_obj = p_taile;

	ObjectManager::GetInstance()->EntrySharedObj(
		std::move(p_taile)
	);

}


void ObjectFactory::CreateTaile2(
	const ObjParameter&data,
	std::weak_ptr<MapObjectBase>p_map_obj
) {

	// 共有用生成
	std::shared_ptr<Taile>p_taile = std::make_shared<Taile>(data);

	ObjectManager::GetInstance()->EntrySharedObj(
		p_taile
	);

	// マップオブジェクト代入
	p_map_obj = p_taile;
}

