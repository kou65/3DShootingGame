#include"EndScene.h"



void EndScene::Update(SceneType&scene_type){

	// シーン終了
	if (KeyBoard::IsKeyPush(DIK_ESCAPE) == true) {
		scene_type = SceneType::SCENE_END;
	}

	if (KeyBoard::IsKeyPush(DIK_RETURN) == true) {
		scene_type = SceneType::TITLE;
	}
}