#pragma once
#include"SceneBase.h"
#include"TitleScene.h"
#include"GameScene.h"
#include"EndScene.h"


// シーンの管理者


class SceneManager {
public:

	SceneManager(
		SceneBase * select_scene,
		const SceneType &scene_type
	);

	void Update() {

		// 現在のシーンを入れる
		SceneType scene_type = m_scene_type;

		// 現在のシーンアップデート(ここでシーンが変化する)
		m_current_scene->Update(scene_type);

		// シーン選択
		SceneSelect(scene_type);
	}

	void Draw() {

	}

private:

	void SceneSelect(SceneType&scene_type) {

		// シーンに変化がないなら
		if (scene_type == m_scene_type) {
			return;
		}

		// シーン削除
		if (m_current_scene != nullptr) {
			delete m_current_scene;
		}

		// シーン生成
		switch (scene_type) {

		case TITLE:
			m_current_scene = new TitleScene;
			break;
		case GAME:
			m_current_scene = new GameScene;
			break;
		case END:
			m_current_scene = new EndScene;
			break;
		}
	}

private:

	SceneType m_scene_type;
	SceneBase *m_current_scene;
};