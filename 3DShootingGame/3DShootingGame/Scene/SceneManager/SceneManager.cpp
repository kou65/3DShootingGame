﻿#include"SceneManager.h"


SceneManager::SceneManager(
	SceneBase * select_scene,
	const SceneType &scene_type
) {

	// 最初のシーン
	m_current_scene = select_scene;

	// シーン
	m_scene_type = scene_type;
}


void SceneManager::Update() {

	// 現在のシーンを入れる
	SceneType scene_type = m_scene_type;

	// 現在のシーンアップデート(ここでシーンが変化する)
	m_current_scene->Update(scene_type);

	// シーン選択
	SceneSelect(scene_type);
}


void SceneManager::Draw() {

	m_current_scene->Draw();
}


void SceneManager::SceneSelect(SceneType&scene_type) {

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

	case SceneType::TITLE:
		m_current_scene = new TitleScene;
		break;
	case SceneType::GAME:
		m_current_scene = new GameScene;
		break;
	case SceneType::END:
		m_current_scene = new EndScene;
		break;
	}
}