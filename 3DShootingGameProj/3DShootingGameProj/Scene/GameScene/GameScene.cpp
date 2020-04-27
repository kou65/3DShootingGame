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
	m_p_camera->Update();

	// オブジェクト更新
	ObjectManager::GetInstance()->Update();

	// 当たり判定更新
	CollisionManager::GetInstance()->Update();

	// 管理更新
	m_p_filed_obj_mng->Update();

	// ゲーム終了判定者更新(あとでjugerにする)
	m_p_game_end_juge_mng->Update(scene_type);

	// 更新
	m_p_map_object_mng->Update();

	// 最後のタイミングで当たり判定オブジェクトを削除させる
	CollisionManager::GetInstance()->CheckAndClear();
}


void GameScene::Draw() {

	// fps値表示
	m_fps.DebugDraw(Vec2(0.f,0.f),200);

	// カメラデバッグ描画
	m_p_camera->TransformDraw();

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
	m_p_obj_factory.reset(new ObjectFactory);

	// オブジェクトデータ生成
	m_p_object_data.reset(new ObjectData);

	// カメラのデータ
	CameraData set_data;

	// 20先に回転軸を置く
	set_data.axis.z = -20.f;
	set_data.pos.x = 16.f;
	set_data.pos.y = 10.f;
	set_data.pos.z = -16.f;

	set_data.is_debug = false;

	m_p_camera.reset(
		new Camera3D(
			Camera3D::FPS,
			set_data
		));

	// ファイル管理者生成
	m_p_file_obj_mng.reset(
		new FileDataManager(
			m_p_obj_factory,
			m_p_camera,
			m_p_object_data
		)
	);

	// 敵管理者生成
	m_p_filed_obj_mng.reset(
		new MapObjectManager(
			m_p_file_obj_mng,
			m_p_obj_factory,
			m_p_object_data
		)
	);

	// 終了管理者生成
	m_p_game_end_juge_mng.reset(
		new GameEndJugeManager(
		m_p_object_data
		)
	);

	// マップオブジェクト管理者生成
	m_p_map_object_mng.reset(
		new MapStructureManager(
			m_p_obj_factory,
			m_p_object_data
		)
	);
}



void GameScene::CreateObject() {

	// UI生成
	m_p_obj_factory->CreateHPUI(m_p_object_data);

}



/*
void GameScene::CreateManager() {

	// 生成
	m_p_mng_factory = std::make_unique<ManagerFactory>();
	m_p_contorl_ofc = std::make_unique<ManagerControlOffcer>();

	std::shared_ptr<FileObjectDataManager>p_file_mng;
	std::shared_ptr<MapObjectManager>p_map_mng;
	std::shared_ptr<MapCharacterManager>p_chara_mng;

	// 管理者生成
	m_p_mng_factory->CreateFileObjectDataManager(
		m_p_obj_factory,
		m_p_camera,
		m_p_object_data,
		p_file_mng
	);

	m_p_mng_factory->CreateMapObjectManager(
		m_p_obj_factory,
		m_p_object_data,
		p_map_mng
	);

	m_p_mng_factory->CreateMapCharacterManager(
		p_file_mng,
		m_p_obj_factory,
		m_p_object_data,
		p_chara_mng
	);
}*/