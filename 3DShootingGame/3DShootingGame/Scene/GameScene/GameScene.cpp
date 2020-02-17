#include"../../GameApp/Player/Player.h"
#include"../../GameApp/Enemy/Enemy.h"
#include"../../GameApp/BulletFactory/BulletFactory.h"
#include"../../Lib\DirectInput\KeyBoard\KeyBoard.h"
#include"../../Lib\Sprite2D\Sprite2D\Sprite2DUser.h"
#include"GameScene.h"
#include"../../GameApp/Filed/Filed.h"



GameScene::GameScene() {

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

	// 弾工場
	m_p_bullet_factory.reset(new BulletFactory(
		&m_object_manager
	));

	// 自機
	m_object_manager.Entry(new Player(
		D3DXVECTOR3(0.f, 0.f, 20.f),
		m_p_camera.get(),
		m_p_bullet_factory.get()));

	// 敵
	m_object_manager.Entry(new Enemy(
		D3DXVECTOR3(0.f, 0.f, 1000.f), 2.f)
	);

	// 部屋
	m_object_manager.Entry(new Filed);
}



GameScene::~GameScene() {
	
}



void GameScene::Update(SceneType&scene_type){

	// カメラ更新
	m_p_camera->Update();

	// オブジェクト更新
	m_object_manager.Update();

}


void GameScene::Draw(){

	// オブジェクト描画
	m_object_manager.Draw();
}