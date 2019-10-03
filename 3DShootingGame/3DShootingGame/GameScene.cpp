#include"GameScene.h"
#include"Filed.h"


GameScene::GameScene() {

	m_p_camera = new Camera3D(Camera3D::FPS);

	BulletFactory * bullet_factory = new BulletFactory(&m_object_manager);

	m_object_manager.Entry(new Player(
		D3DXVECTOR3(0.f, 0.f, 100.f),
		m_p_camera,
		bullet_factory));

	m_object_manager.Entry(new Enemy(
		D3DXVECTOR3(0.f, 0.f, 1000.f), 2.f)
	);

	m_object_manager.Entry(new Filed);
}


void GameScene::Update(SceneType&scene_type){

	// キーボード更新
	KeyBoard::Update();

	m_object_manager.Update();

	m_p_camera->Update();
}


void GameScene::Draw(){

	m_object_manager.Draw();
}