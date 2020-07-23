
#ifndef END_SCENE_H
#define END_SCENE_H
#include"../SceneBase/SceneBase.h"
#include"../../Lib/DirectInput/KeyBoard/KeyBoard.h"



/**
* @brief 終了シーン
*/
class EndScene : public SceneBase {
public:


	/**
	* @brief コンストラクタ
	*/
	EndScene() {}


	/**
	* @brief 更新
	*/
	void Update(SceneType&scene_type);


	/**
	* @brief 描画
	*/
	void Draw()override {};

};
#endif