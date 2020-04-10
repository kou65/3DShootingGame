#pragma once



enum class SceneType {
	TITLE,
	GAME,
	END,
	// ゲームの終了
	SCENE_FINAL
};


class SceneBase {
public:

	SceneBase() {};

	// 仮想デストラクタ
	~SceneBase() {};

	virtual void Update(SceneType&scene_type) = 0;
	virtual void Draw() = 0;
};