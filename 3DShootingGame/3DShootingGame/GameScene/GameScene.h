#pragma once
#include"../SceneBase/SceneBase.h"
#include"../ObjectManager/ObjectManager.h"
#include"../Lib\3D\Camera3D\Camera3D.h"
#include"../Player/Player.h"
#include"../Enemy/Enemy.h"
#include"../BulletFactory/BulletFactory.h"
#include"../Bullet/Bullet.h"
#include"../Lib\DirectInput\KeyBoard\KeyBoard.h"
#include"../Lib\Sprite2D\Sprite2D\Sprite2DObject.h"



class GameScene : public SceneBase {
public:

	GameScene();

	void Update(SceneType&scene_type)override;

	void Draw()override;

private:

	Camera3D * m_p_camera;

	ObjectManager m_object_manager;
};