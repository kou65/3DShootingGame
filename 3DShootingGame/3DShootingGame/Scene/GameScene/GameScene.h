
#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include"../SceneBase/SceneBase.h"
#include"../../ObjectSystem/ObjectManager/ObjectManager.h"
#include"../../Lib\3D\Camera3D\Camera3D.h"
#include"../../ObjectSystem/ObjectFactory/ObjectFactory.h"
#include<memory>



class GameScene : public SceneBase {
public:

	GameScene();

	~GameScene();

	void Update(SceneType&scene_type)override;

	void Draw()override;

private:

	std::unique_ptr<Camera3D>m_p_camera;
	std::unique_ptr<ObjectFactory>m_p_obj_factory;
};


#endif