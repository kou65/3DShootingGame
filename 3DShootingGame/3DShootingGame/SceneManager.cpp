#include"SceneManager.h"


SceneManager::SceneManager(
	SceneBase * select_scene,
	const SceneType &scene_type
) {

	// 最初のシーン
	m_current_scene = select_scene;

	// シーン
	m_scene_type = scene_type;
}