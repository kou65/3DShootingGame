#pragma once
#include"../SceneBase/SceneBase.h"
#include"../../Lib/DirectInput/KeyBoard/KeyBoard.h"



class EndScene : public SceneBase {
public:

	EndScene() {

	}


	void Update(SceneType&scene_type)override {

		// シーン終了
		if (KeyBoard::IsKeyPush(DIK_RETURN) == true) {
			scene_type = SceneType::SCENE_FINAL;
		}
	}

	void Draw()override {

	}
};