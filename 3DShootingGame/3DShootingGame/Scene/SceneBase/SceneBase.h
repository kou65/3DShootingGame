#pragma once



enum class SceneType {
	TITLE,
	GAME,
	END
};

class SceneBase {
public:

	// 仮想デストラクタ
	~SceneBase() {};

	virtual void Update(SceneType&scene_type) = 0;
	virtual void Draw() = 0;
};