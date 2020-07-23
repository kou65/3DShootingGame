#pragma once



/**
* @enum SceneType シーン識別列挙体
*/
enum class SceneType {
	NONE,
	TITLE,
	GAME,
	END,
	// ゲームの終了
	SCENE_END
};


/**
* @brief 基底シーンクラス
*/
class SceneBase {
public:

	/**
	* @brief 基底コンストラクタ
	*/
	SceneBase() {};

	/**
	* @brief 仮想デストラクタ
	*/
	virtual ~SceneBase() {};


	/**
	* @brief 更新
	*/
	virtual void Update(SceneType&scene_type) = 0;


	/**
	* @brief 描画
	*/
	virtual void Draw() = 0;
};