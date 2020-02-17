
#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include"../SceneBase/SceneBase.h"
#include"../../ObjectSystem/ObjectManager/ObjectManager.h"
#include"../../Lib\3D\Camera3D\Camera3D.h"
#include"../../GameApp/BulletFactory/BulletFactory.h"
#include<memory>



class GameScene : public SceneBase {
public:

	GameScene();

	~GameScene();

	void Update(SceneType&scene_type)override;

	void Draw()override;

private:

	std::unique_ptr<Camera3D>m_p_camera;

	std::unique_ptr<BulletFactory>m_p_bullet_factory;

	ObjectManager m_object_manager;
};


#endif