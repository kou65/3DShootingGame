#pragma once
#include"../SceneBase/SceneBase.h"
#include<memory>


/**
* @brief ゲームへの変化シーン
*/
class TransitionScene : public SceneBase{
public:


	/**
	* @brief コンストラクタ
	* @param[in] cur_scene_base 現在のシーン
	*/
	TransitionScene(
		std::unique_ptr<SceneBase>cur_scene_base,
		const SceneType&prev,
		const SceneType&next
	);


	/**
	* @brief 描画
	*/
	void Draw()override;


	/**
	* @brief 更新
	*/
	void Update(SceneType&scene_type)override;


	/**
	* @brief トランジション終了
	*/
	bool IsFadeEnd();


	/**
	* @brief フェードイン終了
	*/
	bool IsFadeInEnd();


	/**
	* @brief フェードアウト終了
	*/
	bool IsFadeOutEnd();

private:

	static constexpr int POWER = 4;

private:


	/**
	* @brief それぞれのシーン移行のフェードを行う
	*/
	void SelectFade(
		SceneType&prev_scene_type,
		SceneType&next_scene_type
	);


	/**
	* @brief フェードイン
	*/
	void BlackBGFadeIn();


	/**
	* @brief フェードアウト
	*/
	void BlackBGFadeOut();


private:

	//! フェードイン終了
	bool m_is_fade_in_end;

	//! フェードアウト終了
	bool m_is_fade_out_end;

	//! α値
	int m_alpha;

	//! 前のシーン種類
	SceneType m_prev_scene_type;

	//! 次のシーン種類
	SceneType m_next_scene_type;

	//! 現在のシーンを保持
	std::unique_ptr<SceneBase>mp_prev_scene;
};