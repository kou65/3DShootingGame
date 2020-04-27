
#ifndef TITLE_SCENE_H
#define TUTLE_SCENE_H

#include"../SceneBase/SceneBase.h"


/**
* @brief タイトルシーン
*/
class TitleScene : public SceneBase{
public:


	/**
	* @brief コンストラクタ
	*/
	TitleScene(){};


	/**
	* @brief 更新 scene_type シーン識別番号 仮想関数
	*/
	void Update(SceneType&scene_type)override;


	/**
	* @brief 描画 仮想関数
	*/
	void Draw()override;


	/**
	* @brief ゲームシーンに切り替え
	*/
	void ChangeGameScene(SceneType&scene_type);

private:


};
#endif