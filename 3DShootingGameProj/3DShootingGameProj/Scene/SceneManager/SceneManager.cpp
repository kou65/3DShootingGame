#include"SceneManager.h"



SceneManager::SceneManager(
	std::unique_ptr<SceneBase>select_scene,
	const SceneType &scene_type
) {

	// 最初のシーン(所有権移動)
	mp_current_scene = std::move(select_scene);

	// シーン
	m_scene_type = scene_type;
	m_next_scene_type = scene_type;

	// 終了
	m_is_end = false;

	// トランジション
	m_is_trans = false;

	m_is_create_scene = false;
}


void SceneManager::Update() {

	// 現在のシーンを入れる
	SceneType scene_type = m_next_scene_type;

	// 現在のシーンが入っているなら
	if (mp_current_scene != nullptr) {

		// 現在のシーンアップデート(ここでシーンが変化する)
		mp_current_scene->Update(scene_type);
	}

	// トランジション中でシーンに変化がないなら
	if (scene_type != m_scene_type){

		// シーン遷移

		// トランス中なら前のトランス情報削除
		if (m_is_trans == true) {

			mp_trans_scene.reset();
			m_is_create_scene = false;
			m_is_trans = false;
		}

		// シーンに変化があるなら
		
		// 変化シーン作成
		CreateTransScene(scene_type);

		// 次のシーンにする
		m_scene_type = m_next_scene_type;
	}

	// トランス中なら更新
	if(m_is_trans == true){

		// トランス中の更新
		UpdateTransScene();
	}
}


void SceneManager::Draw() {


	// 現在のシーンが入っているなら
	if (mp_current_scene != nullptr) {

		mp_current_scene->Draw();
	}

	// トランジション中なら
	if (m_is_trans == true){
		mp_trans_scene->Draw();
	}
}


void SceneManager::UpdateTransScene() {


	if (mp_trans_scene != nullptr) {

		// トランジション中の更新
		mp_trans_scene->Update(m_scene_type);

		// フェードイン終了したなら
		if (mp_trans_scene->IsFadeInEnd() == true) {

			if (mp_current_scene == nullptr) {

				// 予約していたシーンの作成
				CreateScene(m_scene_type, mp_current_scene);
				m_is_create_scene = true;
			}
		}
	}

	// フェードが終了したならトランジション終了
	if (mp_trans_scene->IsFadeEnd() == true) {

		if (m_is_create_scene == true) {

			mp_trans_scene.reset();
			m_is_create_scene = false;
			m_is_trans = false;
		}
	}
}



void SceneManager::CreateTransScene(SceneType&scene_type) {


	// 変化がある場合
	// 次のシーン作成はまだ行わない

	// トランジションシーンがまだ存在しているなら
	if (mp_trans_scene != nullptr){
		mp_trans_scene.reset();
	}

	// シーンに変化があるので
	// トランジションシーン生成(現在のシーン代入)
	mp_trans_scene =
		std::make_unique<TransitionScene>(
			std::move(mp_current_scene),
			m_scene_type,
			scene_type
		);

	// シーン削除
	if (mp_current_scene != nullptr) {
		mp_current_scene.reset();
	}

	// トランジション開始
	m_is_trans = true;

	// 次のシーンタイプを予約
	m_next_scene_type = scene_type;
}



void SceneManager::CreateScene(
	const SceneType&scene_type,
	std::unique_ptr<SceneBase>&scene
	) {


	// シーン生成
	switch (scene_type) {

	case SceneType::TITLE:
		scene =
			std::make_unique<TitleScene>();
		break;
	case SceneType::GAME:
		scene =
			std::make_unique<GameScene>();
		break;
	case SceneType::END:
		scene =
			std::make_unique<EndScene>();
		break;
	case SceneType::SCENE_END:
		m_is_end = true;
		break;
	}

}


bool SceneManager::IsEnd() {
	return m_is_end;
}