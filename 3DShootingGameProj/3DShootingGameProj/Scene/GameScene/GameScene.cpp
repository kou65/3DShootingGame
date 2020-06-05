#include"../../GameApp/Player/Player.h"
#include"../../GameApp/Enemy/Enemy.h"
#include"../../ObjectSystem/ObjectFactory/ObjectFactory.h"
#include"../../Lib\DirectInput\KeyBoard\KeyBoard.h"
#include"../../Lib\Sprite2D\Sprite2D\Sprite2DUser.h"
#include"GameScene.h"
#include"../../Manager/MapStructureManager/MapStructureManager.h"
#include"../../CollisionSystem/CollisionManager/CollisionManager.h"




GameScene::GameScene() {

	Init();
}


GameScene::~GameScene() {
	
}


void GameScene::Update(SceneType&scene_type) {

	// カメラ更新
	mp_camera->Update();

	// 更新
	m_z_tex_user.Update();

	// オブジェクト更新
	ObjectManager::GetInstance()->Update();

	// 当たり判定更新
	CollisionManager::GetInstance()->Update();

	// 管理更新
	mp_filed_obj_mng->Update();

	// ゲーム終了判定者更新(あとでjugerにする)
	mp_game_end_juge_mng->Update(scene_type);

	// 更新
	mp_map_object_mng->Update();

	// 最後のタイミングで当たり判定オブジェクトを削除させる
	CollisionManager::GetInstance()->CheckAndClear();
}


void GameScene::Draw() {

	// fps値表示
	m_fps.DebugDraw(Vec2(0.f,0.f),200);

	// カメラデバッグ描画
	mp_camera->TransformDraw();

	// zテクスチャ使用者
	m_z_tex_user.Draw();

	// オブジェクト描画
	ObjectManager::GetInstance()->Draw();
}


void GameScene::Init() {

	// 操作者生成
	CreateOperator();

	// 管理者生成
	//CreateManager();

	// オブジェクト生成
	CreateObject();

}


void GameScene::CreateOperator() {

	// ファクトリー生成
	mp_obj_factory.reset(new ObjectFactory);

	// オブジェクトデータ生成
	mp_object_data.reset(new ObjectData);

	// カメラのデータ
	CameraData set_data;

	// 20先に回転軸を置く
	set_data.axis.z = -20.f;
	set_data.pos.x = 16.f;
	set_data.pos.y = 10.f;
	set_data.pos.z = -16.f;

	set_data.is_debug = false;

	mp_camera.reset(
		new Camera3D(
			Camera3D::FPS,
			set_data
		));

	// ファイル管理者生成
	mp_file_obj_mng.reset(
		new FileDataManager(
			mp_obj_factory,
			mp_camera,
			mp_object_data
		)
	);

	// 敵管理者生成
	mp_filed_obj_mng.reset(
		new MapObjectManager(
			mp_file_obj_mng,
			mp_obj_factory,
			mp_object_data
		)
	);

	// 終了管理者生成
	mp_game_end_juge_mng.reset(
		new GameEndJugeManager(
		mp_object_data
		)
	);

	// マップオブジェクト管理者生成
	mp_map_object_mng.reset(
		new MapStructureManager(
			mp_obj_factory,
			mp_object_data
		)
	);
}



void GameScene::CreateObject() {

	// UI生成
	mp_obj_factory->CreateHPUI(mp_object_data);

}



/*
void GameScene::CreateManager() {

	// 生成
	mp_mng_factory = std::make_unique<ManagerFactory>();
	mp_contorl_ofc = std::make_unique<ManagerControlOffcer>();

	std::shared_ptr<FileObjectDataManager>p_file_mng;
	std::shared_ptr<MapObjectManager>p_map_mng;
	std::shared_ptr<MapCharacterManager>p_chara_mng;

	// 管理者生成
	mp_mng_factory->CreateFileObjectDataManager(
		mp_obj_factory,
		mp_camera,
		mp_object_data,
		p_file_mng
	);

	mp_mng_factory->CreateMapObjectManager(
		mp_obj_factory,
		mp_object_data,
		p_map_mng
	);

	mp_mng_factory->CreateMapCharacterManager(
		p_file_mng,
		mp_obj_factory,
		mp_object_data,
		p_chara_mng
	);
}*/