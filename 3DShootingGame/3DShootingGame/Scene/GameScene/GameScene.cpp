#include"../../GameApp/Player/Player.h"
#include"../../GameApp/Enemy/Enemy.h"
#include"../../ObjectSystem/ObjectFactory/ObjectFactory.h"
#include"../../Lib\DirectInput\KeyBoard\KeyBoard.h"
#include"../../Lib\Sprite2D\Sprite2D\Sprite2DUser.h"
#include"GameScene.h"
#include"../../GameApp/Filed/Filed.h"
#include"../../CollisionSystem/CollisionManager/CollisionManager.h"




GameScene::GameScene() {

	// ファクトリー生成
	m_p_obj_factory.reset(new ObjectFactory);

	// カメラのデータ
	CameraData set_data;

	// 20先に回転軸を置く
	set_data.axis.z = -20.f;
	//set_data.rota.y = 20.f;
	set_data.pos.y = 15.f;
	set_data.pos.z = -20.f;
	set_data.is_debug = false;

	m_p_camera.reset(
		new Camera3D(
		Camera3D::TPS,
		set_data
	));

	// 自機
	m_p_obj_factory->CreatePlayer(
		Vec3(0.f, 0.f, 20.f),
		m_p_camera.get(),
		m_p_obj_factory.get());

	// 敵
	m_p_obj_factory->CreateEnemy(
		Vec3(0.f, 0.f, 1000.f), 2.f
		);

	// 部屋
	m_p_obj_factory->CreateFiled();
}


GameScene::~GameScene() {
	
}


void GameScene::Update(SceneType&scene_type){

	// カメラ更新
	m_p_camera->Update();

	// オブジェクト更新
	ObjectManager::GetInstance()->Update();

	// 当たり判定更新
	CollisionManager::GetInstance()->Update();
}


void GameScene::Draw(){

	// カメラデバッグ描画
	m_p_camera->TransformDraw();

	// オブジェクト描画
	ObjectManager::GetInstance()->Draw();
}