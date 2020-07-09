#include"GameEndJugeManager.h"



GameEndJugeManager::GameEndJugeManager(
	std::shared_ptr<ObjectData>p_data
) {

	mp_data = p_data;
}


void GameEndJugeManager::Update(SceneType&scene_type) {


	if (mp_data.lock() == nullptr) {
		return;
	}

	std::shared_ptr<Player>p_player =
		mp_data.lock()->p_player.lock();

	std::shared_ptr<GoalObject>p_goal_object =
		mp_data.lock()->p_goal_object.lock();

	if (p_player == nullptr) {
		return;
	}

	// 生きていないなら終了
	if (p_player->IsActive() == false) {
		scene_type = SceneType::END;
	}

	if (p_goal_object == nullptr) {
		return;
	}

	// ゴールオブジェクトが活動を止めたなら終了
	if (p_player->GetPos().z >= p_goal_object->GetPos().z) {

		p_goal_object->Destory();
	}

	if (p_goal_object->IsActive() == false) {
		scene_type = SceneType::END;
	}

}