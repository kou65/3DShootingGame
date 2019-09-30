#pragma once
#include"SceneBase.h"
#include"ObjectManager.h"
#include"Lib\3D\Camera3D\Camera3D.h"
#include"Player.h"
#include"Enemy.h"
#include"BulletFactory.h"
#include"Bullet.h"
#include"Lib\DirectInput\KeyBoard\KeyBoard.h"



class GameScene : public SceneBase {
public:

	GameScene() {

		m_p_camera = new Camera3D(Camera3D::FPS);

		BulletFactory * bullet_factory = new BulletFactory(&m_object_manager);

		m_object_manager.Entry(new Player(
			D3DXVECTOR3(0.f, 0.f, 100.f),
			m_p_camera,
			bullet_factory));

		m_object_manager.Entry(new Enemy(
			D3DXVECTOR3(0.f, 0.f, 1000.f), 2.f
		)
		);

	}

	void Update(SceneType&scene_type)override {

		// キーボード更新
		KeyBoard::Update();

		m_object_manager.Update();

		m_p_camera->Update();
	}

	void Draw()override {

		m_object_manager.Draw();
	}

private:

	Camera3D * m_p_camera;

	ObjectManager m_object_manager;
};