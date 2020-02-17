#pragma once
#include"../SceneBase/SceneBase.h"
#include"../TitleScene/TitleScene.h"
#include"../GameScene/GameScene.h"
#include"../EndScene/EndScene.h"


// シーンの管理者


class SceneManager {
public:

	SceneManager(
		SceneBase * select_scene,
		const SceneType &scene_type
	);

	void Update();

	void Draw();

private:

	void SceneSelect(SceneType&scene_type);

private:

	SceneType m_scene_type;
	SceneBase *m_current_scene;
};