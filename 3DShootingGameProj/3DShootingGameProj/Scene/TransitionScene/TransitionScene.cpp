#include"TransitionScene.h"
#include"../../Lib/Sprite2D/Sprite2D/Sprite2DUser.h"



TransitionScene::TransitionScene(
	std::unique_ptr<SceneBase>cur_scene_base,
	const SceneType&prev,
	const SceneType&next
) {

	if (mp_prev_scene != nullptr) {
		mp_prev_scene.reset();
	}

	// 現在のシーン移動
	mp_prev_scene = std::move(cur_scene_base);

	// シーン種類
	m_prev_scene_type = prev;
	m_next_scene_type = next;

	// フェードイン中
	m_is_fade_in_end = false;
	m_is_fade_out_end = false;

	// 透過から始める
	m_alpha = 0;
}


void TransitionScene::Draw() {

	// フェードインし終わるまで前のシーン描画
	if(m_is_fade_in_end == false){
		mp_prev_scene->Draw();
	}

	// タイトルからゲームの場合
	{
		Sprite2DParameter param;

		param.pos = Vec2(0.f, 0.f);
		param.texture_name = "black_taile";
		param.alpha_value = m_alpha;
		param.tex_size = Vec2(1920.f,1080.f);

		Sprite2DUser::GetInstance()->
			BoardDraw(param);
	}
}


void TransitionScene::Update(SceneType&next_scene_type) {

	SceneType st = SceneType::NONE;

	// フェードを選択してアップデート
	SelectFade(m_prev_scene_type, m_next_scene_type);

	// フェードイン中なら
	if (m_is_fade_in_end == false) {

		mp_prev_scene->Update(st);
	}
	else {

		// フェード終了しているなら削除
		if (m_is_fade_out_end == true) {

			if (mp_prev_scene != nullptr) {
				mp_prev_scene.reset();
			}
		}
	}
}


void TransitionScene::SelectFade(
	SceneType&prev_scene_type,
	SceneType&next_scene_type
) {


	// 前のシーン情報
	switch (prev_scene_type) {

		// 前がタイトル
	case SceneType::TITLE:

		// 次のシーン情報
		switch (next_scene_type) {

			// 次がゲームだったら
		case SceneType::GAME:

			BlackBGFadeIn();
			BlackBGFadeOut();
			break;
		}
		break;

		// 前が終了
	case SceneType::END:

		m_is_fade_in_end = true;
		m_is_fade_out_end = true;
		break;

		// フェードを設定していないならすぐ切り替える
	default:

		m_is_fade_in_end = true;
		m_is_fade_out_end = true;
		break;
	}
}


void TransitionScene::BlackBGFadeIn() {

	if (m_is_fade_in_end == false) {

		if (m_alpha < 255 - POWER) {
			m_alpha+=POWER;
		}
		else {
			// フェードイン終了
			m_is_fade_in_end = true;
		}
	}
}


void TransitionScene::BlackBGFadeOut() {

	// フェードインをしているならフェードアウトを行う
	if (m_is_fade_in_end == true) {

		if (m_alpha > 0) {
			m_alpha-=POWER;
		}
		else {
			// フェードアウト終了
			m_is_fade_out_end = true;
		}
	}
}


bool TransitionScene::IsFadeEnd() {
	return m_is_fade_out_end;
}


bool TransitionScene::IsFadeInEnd() {
	return m_is_fade_in_end;
}


bool TransitionScene::IsFadeOutEnd() {
	return m_is_fade_out_end;
}