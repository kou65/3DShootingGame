#include"GameEndJugeManager.h"



GameEndJugeManager::GameEndJugeManager(
	std::shared_ptr<ObjectData>p_data
) {

	mp_data = p_data;
}


void GameEndJugeManager::Update(SceneType&scene_type) {


	std::shared_ptr<ObjectData>p_data = mp_data.lock();

	if (p_data->p_player == nullptr) {
		return;
	}

	// 生きていないなら終了
	if (p_data->p_player->IsActive() == false) {
		scene_type = SceneType::END;
	}

	if (p_data->p_goal_object == nullptr) {
		return;
	}

	// ゴールオブジェクトが活動を止めたなら終了
	if (p_data->p_player->GetPos().z >= p_data->p_goal_object->GetPos().z) {

		p_data->p_goal_object->Destory();
	}

	if (p_data->p_goal_object->IsActive() == false) {
		scene_type = SceneType::END;
	}

}