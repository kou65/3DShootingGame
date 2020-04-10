#include"../../GameApp/Player/Player.h"
#include"../../GameApp/Enemy/Enemy.h"
#include"../../ObjectSystem/ObjectFactory/ObjectFactory.h"
#include"../../Lib\DirectInput\KeyBoard\KeyBoard.h"
#include"../../Lib\Sprite2D\Sprite2D\Sprite2DUser.h"
#include"GameScene.h"
#include"../../GameApp/Filed/Filed.h"
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
		new FileObjectDataManager(
			m_p_obj_factory.get(),
			m_p_camera.get(),
			m_p_object_data.get()
		)
	);

	// ファイルポインタ
	FILE*p_file = nullptr;

	// ファイル生成
	Utility::FileOpen(&p_file, "FileObject/ObjectList.txt", "r");
	m_p_file_obj_mng->Load(p_file);
	Utility::FileClose(&p_file);


	// 敵管理者生成
	m_p_filed_obj_mng.reset(
		new FiledObjectManager(
			m_p_file_obj_mng.get(),
			m_p_obj_factory.get(),
			m_p_object_data.get()
		)
	);


	// 終了管理者生成
	m_p_game_end_juge_mng.reset(
		new GameEndJugeManager(
		m_p_object_data.get()
		)
	);

}



void GameScene::CreateObject() {

	// ファイルの情報から自機と敵を生成する
	m_p_file_obj_mng->CreateObject();

	// UI生成
	m_p_obj_factory->CreateHPUI(m_p_object_data.get());

	// フィールド生成
	m_p_obj_factory->CreateFiled(m_p_object_data.get());

	//// 敵生成
	//m_p_obj_factory->CreateHEnemy(
	//	Vec3(0.f, 0.f, 200.f),
	//	m_p_object_data.get()->p_player);

}