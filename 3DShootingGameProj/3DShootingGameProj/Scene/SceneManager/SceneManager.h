
#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include"../SceneBase/SceneBase.h"
#include"../TitleScene/TitleScene.h"
#include"../GameScene/GameScene.h"
#include"../EndScene/EndScene.h"



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
		SceneBase * select_scene,
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
	* @brief シーン選択
	* @param[in] scene_type 選択するシーン定数を入れる
	*/
	void SceneSelect(SceneType&scene_type);

private:

	//! シーン識別
	SceneType m_scene_type;

	//! 現在のシーンクラス
	SceneBase *m_current_scene;

	//! 終了したかどうか
	bool m_is_end;
};

#endif