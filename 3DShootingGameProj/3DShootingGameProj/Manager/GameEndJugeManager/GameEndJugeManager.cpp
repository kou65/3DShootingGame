#include"GameEndJugeManager.h"



GameEndJugeManager::GameEndJugeManager(
	ObjectData*p_data
) {

	m_p_data = p_data;
}


void GameEndJugeManager::Update(SceneType&scene_type) {


	if (m_p_data->p_player == nullptr) {
		return;
	}

	// 生きていないなら終了
	if (m_p_data->p_player->IsActive() == false) {
		scene_type = SceneType::END;
	}

	if (m_p_data->p_goal_object == nullptr) {
		return;
	}

	// ゴールオブジェクトが活動を止めたなら終了
	if (m_p_data->p_goal_object->IsActive() == false) {
		scene_type = SceneType::END;
	}

}