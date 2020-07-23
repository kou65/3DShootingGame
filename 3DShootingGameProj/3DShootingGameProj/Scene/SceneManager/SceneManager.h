
#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include"../SceneBase/SceneBase.h"
#include"../TitleScene/TitleScene.h"
#include"../GameScene/GameScene.h"
#include"../EndScene/EndScene.h"
#include"../TransitionScene/TransitionScene.h"
#include<memory>


/**
* @brief シーン
*/
class SceneManager {
public:


	/**
	* @brief コンストラクタ
	* @param[in] select_scene 初期シーンを入れる
	* @param[in] scene_type 現在のシーン定数
	*/
	SceneManager(
		std::unique_ptr<SceneBase>select_scene,
		const SceneType &scene_type
	);


	/**
	* @brief 更新
	*/
	void Update();


	/**
	* @brief 描画
	*/
	void Draw();


	/**
	* @brief 終了したかどうか
	*/
	bool IsEnd();

private:


	/**
	* @brief 変化シーン更新
	*/
	void UpdateTransScene();


	/**
	* @brief シーン選択
	* @param[in] scene_type 選択するシーン定数を入れる
	*/
	void CreateTransScene(SceneType&scene_type);


	/**
	* @brief シーンを生成
	*/
	void CreateScene(
		const SceneType&scene_type,
		std::unique_ptr<SceneBase>&scene
	);

private:

	//! 終了したかどうか
	bool m_is_end;

	//! トランジション中かどうか
	bool m_is_trans;

	//! シーンを生成したかどうか
	bool m_is_create_scene;

	//! 現在のシーン識別
	SceneType m_scene_type;

	//! 前のシーン識別
	SceneType m_next_scene_type;

	//! 現在のシーンクラス
	std::unique_ptr<SceneBase>mp_current_scene;

	//! シーンからシーンへの移行するためのクラス
	std::unique_ptr<TransitionScene>mp_trans_scene;
};

#endif